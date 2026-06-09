# Matrix Class Implementation Guide

**Version:** Complete Implementation with Rule of Five, Move Semantics, and Compound Operators  
**Date:** June 2026  
**Files:**
- Header: [code/matrix.h](../code/matrix.h)
- Implementation: [code/matrix.cpp](../code/matrix.cpp)

---

## Table of Contents

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [The Rule of Five](#the-rule-of-five)
4. [V1: Basic Matrix Implementation](#v1-basic-matrix-implementation)
5. [V2: Move Semantics](#v2-move-semantics)
6. [V3: Compound Operators](#v3-compound-operators)
7. [Operator Summary](#operator-summary)
8. [Memory Management](#memory-management)
9. [Performance Characteristics](#performance-characteristics)
10. [Code Examples](#code-examples)
11. [Statistics Tracking](#statistics-tracking)

---

## Overview

The Matrix class is a comprehensive educational implementation demonstrating:
- **Object-Oriented Design:** Row-major 2D matrix stored as 1D dynamic array
- **Operator Overloading:** 15+ operators for intuitive usage
- **Memory Safety:** Proper RAII (Resource Acquisition Is Initialization)
- **Performance:** Move semantics and compound operators for efficiency
- **Const-Correctness:** Full const/non-const overloading throughout

### Core Principles

| Principle | Implementation | Benefit |
|-----------|----------------|---------|
| **RAII** | Constructor allocates, destructor frees | Exception safety, automatic cleanup |
| **Move Semantics** | Rvalue references (&&) | Avoid unnecessary deep copies |
| **Const-Correctness** | Const versions of accessors | Prevent unintended modifications |
| **Compound Operators** | `+=` and `-=` for in-place ops | Eliminate temporaries in loops |
| **Self-Assignment Safety** | Check `if (this == &other)` | Prevent double-deletion bugs |

---

## Architecture

### Class Structure

```cpp
class Matrix {
private:
    double* data;      // Pointer to heap-allocated 1D array
    int rows_;         // Number of rows
    int cols_;         // Number of columns
    
    // Static counters for performance analysis
    static int instance_count;
    static int copy_count;
    static int move_count;
    
public:
    // Rule of Five: Constructor, Destructor, Copy Constructor, 
    // Copy Assignment, Move Constructor, Move Assignment
    // Plus: Operators, accessors, utilities
};
```

### Memory Layout

For a 3×4 matrix (3 rows, 4 columns):

```
     Col 0  Col 1  Col 2  Col 3
Row 0  [0.0]  [1.0]  [2.0]  [3.0]
Row 1  [4.0]  [5.0]  [6.0]  [7.0]
Row 2  [8.0]  [9.0] [10.0] [11.0]

Data storage (row-major order):
[0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0]
 ↑                                                              ↑
 |______________ single contiguous heap allocation ____________|

Linear index calculation: index(row, col) = row * cols + col
Example: element at (1, 2) = data[1*4 + 2] = data[6] = 6.0
```

### Index Calculation

```cpp
private:
    int index(int row, int col) const {
        return row * cols_ + col;
    }
```

**Why Row-Major?**
- Cache efficiency: Accessing sequential elements loads adjacent cache lines
- Standard in C/C++: Following C conventions for 2D arrays
- Compatible with BLAS/LAPACK: Scientific computing libraries

---

## The Rule of Five

The **Rule of Five** states that if a class manages resources (like dynamic memory), it must explicitly define:

1. **Destructor:** Free allocated resources
2. **Copy Constructor:** Create independent copy
3. **Copy Assignment Operator:** Replace contents safely
4. **Move Constructor:** Efficiently transfer ownership (C++11)
5. **Move Assignment Operator:** Efficiently transfer ownership (C++11)

### Why the Rule of Five Matters

```cpp
// WITHOUT proper implementation (buggy):
Matrix m1(100, 100);
Matrix m2 = m1;           // Shallow copy: both point to same data!
Matrix m3 = m1;           // Triple pointer to same data!
// When m1, m2, m3 destroyed: triple-delete → crash!

// WITH Rule of Five (safe):
Matrix m1(100, 100);
Matrix m2 = m1;           // Deep copy: m2 gets its own data
Matrix m3 = m1;           // m3 gets its own separate data
// When destroyed: each frees its own memory → OK!
```

---

## V1: Basic Matrix Implementation

### Constructor (Part 1 of Rule of Five)

```cpp
Matrix::Matrix(int rows, int cols)
    : rows_(rows), cols_(cols) {
    
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    
    int size = rows * cols;
    data = new double[size];
    
    std::memset(data, 0, size * sizeof(double));  // Fast zero initialization
    
    instance_count++;
}
```

**Key Features:**
- Member initializer list: `: rows_(rows), cols_(cols)`
- Input validation: Reject non-positive dimensions
- Zero initialization: `memset` faster than loop
- Statistics tracking: Increment instance count

### Destructor (Part 2 of Rule of Five)

```cpp
Matrix::~Matrix() {
    if (data != nullptr) {
        delete[] data;        // Free heap allocation
        data = nullptr;       // Prevent use-after-free
    }
    instance_count--;
}
```

**Key Features:**
- Null check: Safe even if constructor failed partway
- `delete[]` for arrays: Not `delete` (single object)
- Nullify pointer: Prevent double-deletion if destructor called twice
- Statistics tracking: Decrement instance count

### Copy Constructor (Part 3 of Rule of Five)

```cpp
Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
    
    int size = rows_ * cols_;
    data = new double[size];
    
    std::memcpy(data, other.data, size * sizeof(double));  // Fast deep copy
    
    instance_count++;
    copy_count++;
}
```

**Key Features:**
- Parameter: `const Matrix& other` (reference to prevent copying)
- Deep copy: Allocate new memory, copy all data
- `memcpy`: Faster than element-wise copy for large arrays
- Statistics: Track copy operations for analysis

**When Called:**
```cpp
Matrix m1(100, 100);
Matrix m2 = m1;              // Copy constructor ← called here
Matrix m3(m1);               // Explicit syntax also calls copy constructor
std::vector<Matrix> v;
v.push_back(m1);             // Copy constructor for vector insertion
```

### Copy Assignment Operator (V1 Enhancement)

```cpp
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;        // Self-assignment check
    }
    
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        delete[] data;       // Free old memory
        
        rows_ = other.rows_;
        cols_ = other.cols_;
        int size = rows_ * cols_;
        data = new double[size];  // Allocate new memory
    }
    
    int size = rows_ * cols_;
    std::memcpy(data, other.data, size * sizeof(double));
    
    copy_count++;
    return *this;
}
```

**Key Features:**
- Parameter: `const Matrix& other` (reference, const-correct)
- Self-assignment check: Prevent memory leak/corruption
- Flexible dimensions: Reallocates if sizes don't match
- Return: `Matrix&` for chaining: `(m1 = m2) = m3`
- Statistics: Track assignment operations

**When Called:**
```cpp
Matrix m1(10, 10);
Matrix m2(20, 20);
m1 = m2;                     // Copy assignment operator ← called here
m1 = m1;                     // Self-assignment (safe - checked above)
m1 = m2 = m3;               // Chaining (right-associative)
```

**Self-Assignment Prevention:**

Why is the self-assignment check critical?

```cpp
// WITHOUT check (buggy):
m = m;  // this == &other
delete[] data;           // FREE OUR OWN MEMORY! ← Bug!
data = new double[...];  // Allocate new
memcpy(data, other.data, ...);  // Copy from freed memory! ← Crash!

// WITH check (safe):
if (this == &other) {
    return *this;       // Skip everything
}
// Safe to proceed
```

---

## V2: Move Semantics

Move semantics enable efficient transfer of resources from temporaries without deep copying.

### Move Constructor (Part 4 of Rule of Five)

```cpp
Matrix::Matrix(Matrix&& other) noexcept
    : data(other.data), rows_(other.rows_), cols_(other.cols_) {
    
    other.data = nullptr;    // Nullify source
    other.rows_ = 0;
    other.cols_ = 0;
    
    instance_count++;
    move_count++;
}
```

**Key Components:**

| Component | Purpose |
|-----------|---------|
| `Matrix&&` | Rvalue reference - accepts temporaries |
| `noexcept` | Guarantee no exceptions (required for move) |
| Pointer theft | Copy pointers, not data (O(1) instead of O(n²)) |
| Nullify source | Prevent double-deletion when source destroyed |

**Rvalue References: When They Bind**

```cpp
Matrix temp = Matrix(100, 100);    // Temporary ← rvalue, move constructor called
Matrix m1 = std::move(m2);         // Explicit move ← m2 is now rvalue
Matrix m3 = m2;                    // m2 is lvalue ← copy constructor called

// Function returns temporary (implicit move):
Matrix compute() {
    Matrix result(100, 100);
    // ... compute ...
    return result;  // Temporary rvalue ← move constructor called
}

Matrix m = compute();  // Receives moved object (no copy!)
```

**Move Constructor vs Copy Constructor Performance:**

```
Copy Constructor (100×100 matrix = 80 KB):
    1. Allocate new array:  ~50 ns
    2. memcpy 80 KB:        ~689 ns
    Total:                  ~740 ns

Move Constructor (100×100 matrix):
    1. Copy 3 pointers:     ~1 ns
    2. Nullify pointers:    ~1 ns
    Total:                  ~2 ns

Speedup: 740 ns / 2 ns = 370× faster!
```

### Move Assignment Operator (Part 5 of Rule of Five)

```cpp
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this == &other) {
        return *this;        // Prevent: m = std::move(m);
    }
    
    delete[] data;           // Free old memory
    
    data = other.data;       // Steal ownership
    rows_ = other.rows_;
    cols_ = other.cols_;
    
    other.data = nullptr;    // Nullify source
    other.rows_ = 0;
    other.cols_ = 0;
    
    move_count++;
    return *this;
}
```

**Key Features:**
- Parameter: `Matrix&&` (rvalue reference)
- Self-assignment check: Safety first
- Memory cleanup: Free old data before replacing
- Pointer theft: O(1) operation
- Nullify source: Safe destruction of moved-from object
- Return: `Matrix&` for chaining

**When Called:**
```cpp
Matrix m1(10, 10);
m1 = Matrix(100, 100);      // Temporary rvalue ← move assignment called
m1 = std::move(m2);         // Explicit move ← move assignment called
m1 = m1;                    // Self-assignment (safe - checked)

// After move:
m2.rows() == 0;             // m2 is now empty (moved-from state)
m2.cols() == 0;
```

### Moved-From State

After a move operation, the source object is in a "moved-from" state:

```cpp
Matrix source(100, 100);
Matrix dest = std::move(source);

// source is now moved-from:
source.rows() == 0;         // Valid but empty
source.cols() == 0;
// source.data == nullptr is internal, not directly accessible

// Can't use source for computation:
Matrix result = source + dest;  // BAD! source is empty
source.fill(5.0);               // BAD! No data to fill

// Can reassign source:
source = Matrix(50, 50);    // ✓ OK - move assignment from temporary
```

**Design:** Moved-from objects are in a valid but unspecified state, allowing safe destruction.

---

## V3: Compound Operators

Compound operators (`+=`, `-=`) modify objects in-place, avoiding temporary creation.

### Member Operator: += (In-Place Addition)

```cpp
Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] += other.data[i];  // Modify in-place
    }
    
    return *this;  // Allow chaining: (A += B) += C
}
```

**Key Features:**
- Returns: Reference to `*this` (not a copy)
- Modifies: Original matrix is changed
- No temporaries: No intermediate results created
- Dimension validation: Ensures safety

**Performance Comparison:**

```cpp
// Method 1: Using += (Best)
Matrix result = A;
result += B;
// Performance: No temporaries created
// Operations: 0 copies, 0 moves

// Method 2: Using + operator (Good)
Matrix result = A + B;
// Performance: Creates temporary via operator+
// Operations: 1 copy (A), 1 move (return optimization)

// Method 3: Using = (Worst)
Matrix result = A;
result = result + B;
// Performance: Creates temporary
// Operations: 1 copy (A), 1 copy (assignment)
```

### Member Operator: -= (In-Place Subtraction)

```cpp
Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] -= other.data[i];  // Modify in-place
    }
    
    return *this;
}
```

Same structure as `+=` but for subtraction.

### Free Functions: operator+ and operator-

These are implemented using the compound operators (DRY principle):

```cpp
// Free function addition: Uses += member operator
Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;      // Copy constructor
    result += rhs;            // Member operator +=
    return result;            // Move semantics optimizes return
}

// Free function subtraction: Uses -= member operator
Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;      // Copy constructor
    result -= rhs;            // Member operator -=
    return result;            // Move semantics optimizes return
}
```

**Benefits:**
- **DRY:** Addition algorithm defined once (in `+=`)
- **Consistency:** Both `+` and `+=` use identical logic
- **Simplicity:** Less code, fewer bugs
- **Performance:** Move semantics make return efficient

### Multiplication Operator: * (Non-Compound)

Multiplication can't be compound (C = C * B is not equivalent to C *= B with different semantics):

```cpp
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Inner dimensions must match");
    }
    
    Matrix result(rows_, other.cols_);
    
    // Standard O(n³) matrix multiplication
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_; ++k) {
                sum += data[index(i, k)] * other.data[other.index(k, j)];
            }
            result.data[result.index(i, j)] = sum;
        }
    }
    
    return result;  // Moved via RVO
}
```

**Key Features:**
- `const` member function: Doesn't modify `*this`
- Dimension check: Inner dimensions must match (m×n * n×p = m×p)
- Triple-nested loop: O(n³) algorithm
- Return: New matrix created, moved to caller

---

### Equality Operator: == (Complete Implementation)

```cpp
bool Matrix::operator==(const Matrix& other) const {
    // Check dimensions first
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    // Compare elements with tolerance for floating-point
    const double EPSILON = 1e-9;
    int size = rows_ * cols_;
    
    for (int i = 0; i < size; ++i) {
        if (std::abs(data[i] - other.data[i]) > EPSILON) {
            return false;
        }
    }
    
    return true;
}
```

**Key Features:**
- Parameter: `const Matrix&` (const reference, read-only)
- `const` member function: Doesn't modify either object
- Returns: `bool` (true if equal, false otherwise)
- Dimension check: Ensures matrices have same shape
- Floating-point tolerance: Uses EPSILON = 1e-9 to handle floating-point precision issues
- Early exit: Returns false as soon as mismatch found

**Why Floating-Point Tolerance?**

```cpp
// WITHOUT tolerance (WRONG):
double a = 0.1 + 0.2;              // 0.30000000000000004 (floating-point error)
double b = 0.3;                    // 0.3
if (a == b) /* false! Even though logically they're the same */

// WITH tolerance (CORRECT):
if (std::abs(a - b) < 1e-9) /* true! Accounts for floating-point precision */
```

---

### Subscript Operator: [] (Row Access - Complete Implementation)

Non-const version (read/write):
```cpp
double* Matrix::operator[](int row) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}
```

Const version (read-only):
```cpp
const double* Matrix::operator[](int row) const {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}
```

**Key Features:**
- Parameter: `int row` (row index, 0-based)
- Returns: Pointer to first element of the row
- Bounds checking: Throws `out_of_range` if index invalid
- Row-major access: Calculates pointer as `&data[row * cols_]`
- Two versions: Const and non-const for const-correctness

**Usage Examples:**

```cpp
Matrix m(10, 10);
m.fill(5.0);

// Direct element modification via pointer
m[2][3] = 7.5;              // Row 2, column 3
double val = m[0][0];       // Read row 0, column 0

// Iterate over row
for (int j = 0; j < m.cols(); ++j) {
    m[0][j] = j * 0.1;      // Fill first row
}

// Const matrix - returns const pointer
const Matrix& cm = m;
double x = cm[0][0];        // ✓ OK - read-only
// cm[0][0] = 5.0;          // ✗ ERROR - const pointer assignment
```

**Performance:** O(1) - immediate pointer arithmetic, no bounds check overhead (unlike `at()`)

---

### Stream Insertion Operator: << (Complete Implementation)

```cpp
std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    os << "Matrix " << mat.rows() << "x" << mat.cols() << ":\n";
    
    for (int i = 0; i < mat.rows(); ++i) {
        os << "  [ ";
        for (int j = 0; j < mat.cols(); ++j) {
            os << std::fixed << std::setw(8) << std::setprecision(2) 
               << mat(i, j) << " ";
        }
        os << "]\n";
    }
    
    return os;
}
```

**Key Features:**
- Free function (not member): `friend` access to Matrix
- First parameter: `std::ostream&` (output stream - std::cout, std::ofstream, etc.)
- Second parameter: `const Matrix&` (matrix to print)
- Returns: `std::ostream&` (allows chaining: `std::cout << m1 << m2 << m3`)
- Formatting:
  - `std::fixed`: Fixed-point notation (not scientific)
  - `std::setw(8)`: Column width of 8 characters
  - `std::setprecision(2)`: 2 decimal places
  - Row labels: `[ ... ]` for readability

**Usage Examples:**

```cpp
Matrix m(3, 3);
m.fill(2.5);

// Direct printing
std::cout << m;
// Output:
// Matrix 3x3:
//   [    2.50    2.50    2.50 ]
//   [    2.50    2.50    2.50 ]
//   [    2.50    2.50    2.50 ]

// Chaining
std::cout << "Matrix A:\n" << m1 << "Matrix B:\n" << m2;

// File output
std::ofstream file("matrix.txt");
file << m;
```

**Note:** This operator is a free function, not a member, because the left operand is `std::ostream`, which the Matrix class doesn't own.

**Key Features:**
- `const` member function: Doesn't modify `*this`
- Dimension check: Inner dimensions must match (m×n * n×p = m×p)
- Triple-nested loop: O(n³) algorithm
- Return: New matrix created, moved to caller

---

## Operator Summary

### Complete Operator List

| Category | Operator | Type | Method | Notes |
|----------|----------|------|--------|-------|
| **Construction** | Constructor | Member | `Matrix(int, int)` | Allocates and zeros |
| **Construction** | Destructor | Member | `~Matrix()` | Deallocates memory |
| **Construction** | Copy Constructor | Member | `Matrix(const Matrix&)` | Deep copy |
| **Construction** | Copy Assignment | Member | `operator=(const Matrix&)` | Safe assignment |
| **Construction** | Move Constructor | Member | `Matrix(Matrix&&)` | Efficient transfer |
| **Construction** | Move Assignment | Member | `operator=(Matrix&&)` | Efficient transfer |
| **Access** | Subscript | Member | `operator[]` | Row access |
| **Access** | Function call | Member | `operator()` | Element access |
| **Access** | at() | Member | `at(row, col)` | Bounds-checked access |
| **Arithmetic** | Addition | Free | `operator+` | Creates temporary |
| **Arithmetic** | Subtraction | Free | `operator-` | Creates temporary |
| **Arithmetic** | Multiplication | Member | `operator*` | O(n³) algorithm |
| **Arithmetic** | In-place Add | Member | `operator+=` | Modifies in-place |
| **Arithmetic** | In-place Sub | Member | `operator-=` | Modifies in-place |
| **Comparison** | Equality | Member | `operator==` | Floating-point tolerant |
| **Stream** | Output | Free | `operator<<` | Formatted printing |

### Const-Correctness Examples

```cpp
// Both at() versions:
const double& at(int row, int col) const;  // For const matrices
double& at(int row, int col);              // For non-const matrices

// Both operator() versions:
const double& operator()(int row, int col) const;  // Read-only
double& operator()(int row, int col);              // Read/write

// Both operator[] versions:
const double* operator[](int row) const;   // Read-only row access
double* operator[](int row);                // Read/write row access

// Const member functions (don't modify *this):
Matrix operator*(const Matrix& other) const;  // Multiplication
bool operator==(const Matrix& other) const;   // Comparison
int rows() const { return rows_; }            // Getter
```

---

## Memory Management

### Manual Memory Control via RAII

The Matrix class uses RAII (Resource Acquisition Is Initialization):

```cpp
// Resource Acquisition: Constructor
Matrix::Matrix(int rows, int cols) {
    data = new double[rows * cols];  // Acquire memory
    instance_count++;
}

// Resource Release: Destructor
Matrix::~Matrix() {
    delete[] data;                   // Release memory
    instance_count--;
}
```

**Advantages:**
- Automatic cleanup: No manual `delete` needed
- Exception safety: Destructor called even if exception thrown
- Scope-based: Objects cleaned up when going out of scope

### Memory Lifecycle Example

```cpp
{
    Matrix m1(100, 100);           // Constructor: allocates 80 KB
    {
        Matrix m2 = m1;            // Copy constructor: allocates 80 KB
        Matrix m3 = std::move(m2);  // Move constructor: no allocation
    }  // Destructors called: m3 freed (80 KB), m2 freed (80 KB)
}  // Destructor called: m1 freed (80 KB)

// All memory properly cleaned up - no leaks!
```

### Self-Assignment Safety Pattern

```cpp
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;  // Prevent: m = m;
    }
    
    // Safe to proceed: we know this != &other
    delete[] data;     // Free old memory
    // ... allocate new, copy data ...
    return *this;
}
```

### Move-Assignment Self-Assignment Safety

```cpp
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this == &other) {
        return *this;  // Prevent: m = std::move(m);
    }
    
    delete[] data;     // Free old data
    data = other.data;
    other.data = nullptr;  // Nullify source
    
    return *this;
}
```

Why important: `m = std::move(m)` would delete our own memory!

---

## Performance Characteristics

### Operation Complexity

| Operation | Complexity | Time (100×100) |
|-----------|------------|----------------|
| Constructor | O(n²) | ~2 µs |
| Destructor | O(1) | ~10 ns |
| Copy Constructor | O(n²) | ~740 ns |
| Copy Assignment | O(n²) | ~740 ns |
| Move Constructor | O(1) | ~2 ns |
| Move Assignment | O(1) | ~2 ns |
| Element Access `()` | O(1) | ~1 ns |
| Element Access `at()` | O(1) | ~2 ns (with bounds check) |
| Addition `+` | O(n²) | ~3 µs |
| In-Place Add `+=` | O(n²) | ~2 µs |
| Multiplication `*` | O(n³) | ~61 µs |

### Memory Overhead per Matrix

```
Fixed overhead:
  - 3 integers: rows, cols, padding = 16 bytes (aligned)
  
Variable overhead:
  - Pointer to data: already counted in fixed
  - Data array: 8 * rows * cols bytes (double = 8 bytes)

Example (100×100):
  - Fixed: 16 bytes
  - Data: 8 * 100 * 100 = 80,000 bytes
  - Total: ~80 KB per instance
```

### Optimization Opportunities

1. **SSE/AVX vectorization:** Hand-written SIMD for addition/subtraction
2. **Cache blocking:** For large multiplication (currently O(n³) with poor cache locality)
3. **Expression templates:** Compile-time elimination of temporaries
4. **Specialized kernels:** BLAS/LAPACK integration

---

## Code Examples

### Example 1: Basic Usage

```cpp
#include "matrix.h"

int main() {
    // Create two 100×100 matrices
    Matrix A(100, 100);
    Matrix B(100, 100);
    
    A.fill(2.5);
    B.fill(3.7);
    
    // Addition (creates temporary)
    Matrix result = A + B;
    result.print("A + B");
    
    // In-place addition (no temporary)
    A += B;
    A.print("A += B");
    
    // Multiplication
    Matrix product = A * B;
    product.print("A * B");
    
    return 0;
}
```

### Example 2: Move Semantics

```cpp
#include "matrix.h"

Matrix computeMatrix() {
    Matrix temp(100, 100);
    // ... compute result ...
    return temp;  // Move constructor used (no copy!)
}

int main() {
    Matrix result = computeMatrix();
    // result efficiently receives the matrix from function
    // No copy of 80 KB data!
    
    return 0;
}
```

### Example 3: Compound Operators in Loops

```cpp
#include "matrix.h"

int main() {
    Matrix accumulator(100, 100);
    accumulator.fill(0.0);
    
    Matrix::resetStats();
    
    // Loop 1000 times, accumulating
    for (int i = 0; i < 1000; ++i) {
        Matrix temp(100, 100);
        temp.fill(1.0 / 1000);
        
        // BEST: No temporary created
        accumulator += temp;
    }
    
    Matrix::printStats();
    // Output:
    // Total Copies: 0
    // Total Moves:  0
    // (temp objects destructed, but no operations on accumulator)
    
    return 0;
}
```

### Example 4: Copy vs Move

```cpp
#include "matrix.h"
#include <iostream>

int main() {
    Matrix m1(100, 100);
    m1.fill(1.5);
    
    Matrix::resetStats();
    
    // Copy semantics
    Matrix m2 = m1;  // Copy constructor: copy_count = 1
    std::cout << "Copies: " << Matrix::getCopyCount() << "\n";
    
    // Move semantics
    Matrix::resetStats();
    Matrix m3 = std::move(m1);  // Move constructor: move_count = 1
    std::cout << "Moves: " << Matrix::getMoveCount() << "\n";
    
    // m1 is now moved-from (empty)
    std::cout << "m1 dimensions: " << m1.rows() << "x" << m1.cols() << "\n";
    
    return 0;
}
```

### Example 5: Error Handling

```cpp
#include "matrix.h"
#include <iostream>

int main() {
    try {
        // Invalid dimensions
        Matrix m(0, 100);  // Throws invalid_argument
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    try {
        // Dimension mismatch
        Matrix A(10, 10);
        Matrix B(20, 20);
        Matrix C = A + B;  // Throws invalid_argument
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    try {
        // Out of bounds
        Matrix m(10, 10);
        double x = m.at(15, 15);  // Throws out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    return 0;
}
```

---

## Statistics Tracking

The Matrix class includes built-in performance statistics:

### Static Member Variables

```cpp
private:
    static int instance_count;  // Active matrix instances
    static int copy_count;      // Total deep copies
    static int move_count;      // Total move operations
```

### API for Statistics

```cpp
// Get current counts
int instances = Matrix::getInstanceCount();
int copies = Matrix::getCopyCount();
int moves = Matrix::getMoveCount();

// Reset counters
Matrix::resetStats();

// Print formatted output
Matrix::printStats();
```

### Example Statistics Output

```
========== Matrix Statistics ==========
Active Instances: 5
Total Copies:     127
Total Moves:      340
========================================
```

### Using Statistics for Benchmarking

```cpp
#include "matrix.h"

int main() {
    Matrix::resetStats();
    
    // Benchmark section
    for (int i = 0; i < 1000; ++i) {
        Matrix temp(100, 100);
        temp.fill(i * 0.001);
        
        Matrix result = temp + temp;  // Creates temporary
    }
    
    Matrix::printStats();
    // See how many copies/moves happened
    
    return 0;
}
```

---

## Comparison with Standard Library

### Matrix vs std::vector<std::vector<double>>

| Feature | Matrix Class | std::vector |
|---------|--------------|-------------|
| **Memory Layout** | Contiguous 1D | Non-contiguous (pointers) |
| **Cache Efficiency** | Excellent | Poor (pointer chasing) |
| **Move Semantics** | Full support | Yes, built-in |
| **Operator Overloading** | Math operators | Generic |
| **Size Overhead** | ~16 bytes | ~24 bytes per dimension |
| **Use Case** | Numerical computing | General purpose |

### Matrix vs Eigen (Production Library)

| Feature | Our Matrix | Eigen |
|---------|-----------|-------|
| **Performance** | Good for demonstrations | Optimized (SIMD, blocking) |
| **Features** | Basic operations | 200+ operations |
| **Template Expressions** | Simple | Expression templates (zero-copy) |
| **GPU Support** | No | Via plugins |
| **Production Ready** | Educational | Industry standard |

---

## Best Practices When Using Matrix

### ✅ DO:

```cpp
// Use compound operators in loops
for (int i = 0; i < 1000; ++i) {
    result += temp;  // Efficient
}

// Use move semantics for returns
Matrix compute() {
    Matrix m(100, 100);
    // ... computation ...
    return m;  // Move constructor used
}

// Check statistics for optimization
Matrix::resetStats();
// ... operations ...
Matrix::printStats();  // See if copies/moves excessive

// Use const-correct references
void process(const Matrix& input);  // Prevent accidental copies
```

### ❌ DON'T:

```cpp
// Don't accumulate with +
for (int i = 0; i < 1000; ++i) {
    result = result + temp;  // Creates unnecessary temporary
}

// Don't copy large matrices
void process(Matrix m);  // Parameter should be const&
process(large_matrix);   // Expensive copy!

// Don't assume moved-from objects are usable
Matrix m = compute();
m.fill(5.0);             // OK
Matrix m2 = std::move(m);
m.fill(5.0);             // BAD! m is moved-from (empty)
```

---

## Conclusion

The Matrix class demonstrates **modern C++ best practices**:

1. **Complete Rule of Five:** All resource management covered
2. **Move Semantics:** Efficient temporary handling
3. **Compound Operators:** In-place modifications
4. **Const-Correctness:** Prevent unintended modifications
5. **Error Handling:** Exception safety
6. **Performance Tracking:** Statistics for optimization
7. **Educational:** Well-commented code

This implementation serves as a template for creating efficient, safe numerical computing classes in C++.

---

## File References

- Header with all declarations: [code/matrix.h](../code/matrix.h)
- Implementation with explanations: [code/matrix.cpp](../code/matrix.cpp)
- Benchmark suite: [code/matrix_google_benchmark.cpp](../code/matrix_google_benchmark.cpp)
- Heap usage analysis: [code/heap_comparison.cpp](../code/heap_comparison.cpp)
- Stats logging: [code/matrix_google_benchmark_with_stats.cpp](../code/matrix_google_benchmark_with_stats.cpp)

---

**Version:** C++17 Standard  
**Compiler:** LLVM/Clang 22.1.6  
**Last Updated:** June 2026
