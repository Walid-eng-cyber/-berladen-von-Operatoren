# Matrix Class Implementation Documentation

## Overview

The **Matrix class** is a comprehensive C++ implementation demonstrating operator overloading, memory management, and the Rule of Five. It provides a 2D matrix data structure with full support for arithmetic operations, element access, and formatted output.

## Table of Contents

1. [Features](#features)
2. [Class Design](#class-design)
3. [Memory Management](#memory-management)
4. [Operator Overloads](#operator-overloads)
5. [Usage Examples](#usage-examples)
6. [Building and Running](#building-and-running)
7. [Testing](#testing)
8. [Performance Considerations](#performance-considerations)

---

## Features

### Core Functionality
- ✅ **Dynamic 2D Matrix Storage**: Efficient row-major order storage using 1D dynamic arrays
- ✅ **Proper Memory Management**: Constructor, destructor, copy constructor, and copy assignment
- ✅ **Self-Assignment Detection**: Optimized for safe self-assignment operations
- ✅ **Dimension Flexibility**: Support for any NxM matrix dimensions
- ✅ **Bounds Checking**: Optional bounds-checked element access via `at()` method
- ✅ **Statistics Tracking**: Runtime tracking of object creation, copying, and destruction

### Operator Support
- `+` Matrix addition
- `-` Matrix subtraction
- `*` Matrix multiplication
- `==` Equality comparison
- `[]` Row access
- `<<` Stream insertion (formatted output)
- `()` Element access (unchecked)

### Additional Features
- `fill()` Fill all elements with a single value
- `print()` Formatted console output with debug information
- `at()` Bounds-checked element access with exception throwing
- Statistics tracking with `resetStats()` and `printStats()`

---

## Class Design

### Header File Structure (`matrix.h`)

```cpp
class Matrix {
private:
    double* data;        // 1D dynamic array storing matrix data
    int rows_;           // Number of rows
    int cols_;           // Number of columns
    
    // Static statistics counters
    static int instance_count;
    static int copy_count;
    static int move_count;

public:
    // Rule of Five implementations
    Matrix(int rows, int cols);           // Constructor
    ~Matrix();                            // Destructor
    Matrix(const Matrix& other);          // Copy Constructor
    Matrix& operator=(const Matrix& other); // Copy Assignment
    
    // ... operator overloads and methods ...
};
```

### Memory Layout

The matrix uses **row-major order** storage, where elements are stored sequentially by rows:

```
Matrix M(3, 4):
  [a₀₀ a₀₁ a₀₂ a₀₃ | a₁₀ a₁₁ a₁₂ a₁₃ | a₂₀ a₂₁ a₂₂ a₂₃]
   └─ Row 0 ────────  └─ Row 1 ────────  └─ Row 2 ────────
```

Linear index calculation: `index(i, j) = i * cols + j`

---

## Memory Management

### Constructor

```cpp
Matrix::Matrix(int rows, int cols)
    : rows_(rows), cols_(cols) {
    
    int size = rows * cols;
    data = new double[size];
    std::memset(data, 0, size * sizeof(double));
    instance_count++;
}
```

- Allocates memory for all elements
- Initializes all elements to 0.0
- Increments instance counter

### Destructor

```cpp
Matrix::~Matrix() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    instance_count--;
}
```

- Properly deallocates dynamically allocated memory
- Prevents memory leaks
- Updates statistics

### Copy Constructor (Deep Copy)

```cpp
Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
    
    int size = rows_ * cols_;
    data = new double[size];
    std::memcpy(data, other.data, size * sizeof(double));
    
    instance_count++;
    copy_count++;
}
```

- Creates independent copy of matrix data
- Modifying copy doesn't affect original
- Tracks number of copies

### Copy Assignment (Self-Assignment Safe)

```cpp
Matrix& Matrix::operator=(const Matrix& other) {
    // Self-assignment check
    if (this == &other) {
        return *this;
    }
    
    // Handle dimension mismatch
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        delete[] data;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data = new double[rows_ * cols_];
    }
    
    // Copy data
    std::memcpy(data, other.data, rows_ * cols_ * sizeof(double));
    copy_count++;
    return *this;
}
```

**Key Optimizations:**
- Checks for self-assignment before processing
- Reuses memory if dimensions match
- Allocates new memory only when necessary

---

## Operator Overloads

### 1. Addition Operator (`operator+`)

```cpp
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}
```

**Usage:**
```cpp
Matrix C = A + B;  // Element-wise addition
```

**Complexity:** O(n×m) where n=rows, m=cols

### 2. Subtraction Operator (`operator-`)

```cpp
Matrix Matrix::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_ * cols_; ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}
```

**Usage:**
```cpp
Matrix D = C - A;  // Element-wise subtraction
```

**Complexity:** O(n×m)

### 3. Multiplication Operator (`operator*`)

```cpp
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Inner dimensions must match");
    }
    
    Matrix result(rows_, other.cols_);
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_; ++k) {
                sum += data[index(i, k)] * other.data[other.index(k, j)];
            }
            result.data[result.index(i, j)] = sum;
        }
    }
    
    return result;
}
```

**Usage:**
```cpp
Matrix E = A * B;  // A(m×n) * B(n×p) = E(m×p)
```

**Complexity:** O(m×n×p) for (m×n) × (n×p) matrices

**Mathematical Formula:**
```
C[i][j] = Σ(k=0 to n-1) A[i][k] × B[k][j]
```

### 4. Equality Operator (`operator==`)

```cpp
bool Matrix::operator==(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    const double EPSILON = 1e-9;
    for (int i = 0; i < rows_ * cols_; ++i) {
        if (std::abs(data[i] - other.data[i]) > EPSILON) {
            return false;
        }
    }
    
    return true;
}
```

**Usage:**
```cpp
if (A == B) {
    std::cout << "Matrices are equal\n";
}
```

**Features:**
- Dimension comparison first
- Floating-point tolerance (epsilon = 1e-9)
- Element-wise comparison

### 5. Subscript Operator (`operator[]`)

```cpp
double* Matrix::operator[](int row) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}

const double* Matrix::operator[](int row) const {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}
```

**Usage:**
```cpp
double* row = M[1];           // Get pointer to row 1
double value = M[1][2];       // Access element [1][2]
row[3] = 5.5;                 // Modify through row pointer
```

**Features:**
- Returns pointer to row data for efficient access
- Bounds checking with exception throwing
- Both const and non-const versions

### 6. Stream Insertion Operator (`operator<<`)

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

**Usage:**
```cpp
std::cout << A;  // Formatted output
```

**Example Output:**
```
Matrix 2x3:
  [     1.00     2.00     3.00 ]
  [     4.00     5.00     6.00 ]
```

---

## Usage Examples

### Basic Construction and Initialization

```cpp
#include "matrix.h"

int main() {
    // Create matrices
    Matrix A(3, 3);      // 3x3 zero matrix
    A.fill(1.5);         // Fill with 1.5
    
    Matrix B(3, 3);
    B(0, 0) = 2.0;
    B(1, 1) = 3.0;
    B(2, 2) = 4.0;
    
    return 0;
}
```

### Arithmetic Operations

```cpp
Matrix A(2, 2), B(2, 2), C(2, 2);
A.fill(1.0);
B.fill(2.0);

// Addition
C = A + B;  // [3, 3; 3, 3]

// Subtraction
C = B - A;  // [1, 1; 1, 1]

// Multiplication
Matrix D(2, 3);
Matrix E(3, 2);
Matrix F = D * E;  // Results in 2x2 matrix
```

### Element Access

```cpp
Matrix M(3, 4);

// Unchecked access (faster)
M(0, 0) = 5.5;
double val = M(1, 2);

// Bounds-checked access
try {
    double val = M.at(5, 5);  // Throws if out of bounds
} catch (const std::out_of_range& e) {
    std::cerr << "Error: " << e.what() << "\n";
}

// Row-based access
double* row = M[1];
double elem = M[1][2];
```

### Comparison and Output

```cpp
Matrix A(2, 2), B(2, 2);
A.fill(3.0);
B = A;

if (A == B) {
    std::cout << "Matrices are equal\n";
}

std::cout << A;  // Formatted output
```

---

## Building and Running

### Compilation

**Using clang++ (LLVM-mingw):**
```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
clang++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 code\matrix.cpp code\matrix_demo.cpp -o bin\matrix_demo.exe
```

**Using g++ (MinGW):**
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 code/matrix.cpp code/matrix_demo.cpp -o bin/matrix_demo.exe
```

### Running Programs

**Matrix Demo (Shows all features):**
```powershell
.\bin\matrix_demo.exe
```

**Operator Demo (Focused on overloads):**
```powershell
.\bin\operator_demo.exe
```

**Unit Tests:**
```powershell
.\bin\matrix_tests.exe
```

---

## Testing

### Unit Test Suite

A comprehensive test suite (`matrix_tests.cpp`) validates all functionality:

**Test Coverage (56 tests total):**
- Constructor and accessors (4 tests)
- Copy operations (12 tests)
- Operator+ addition (7 tests)
- Operator- subtraction (6 tests)
- Operator* multiplication (7 tests)
- Operator== equality (5 tests)
- Operator[] row access (6 tests)
- at() bounds checking (4 tests)
- Operator<< stream output (3 tests)
- Complex operations (2 tests)

**Running Tests:**
```powershell
.\bin\matrix_tests.exe
```

**Example Output:**
```
Total Tests:   56
Passed:        56 ✓
Failed:        0 ✗

🎉 ALL TESTS PASSED!
```

### Key Test Cases

1. **Deep Copy Verification**: Modifying a copy doesn't affect the original
2. **Dimension Handling**: Proper validation of matrix dimensions for operations
3. **Self-Assignment Safety**: `M = M` doesn't cause issues
4. **Floating-Point Tolerance**: `operator==` uses epsilon for comparison
5. **Exception Handling**: Out-of-bounds access throws exceptions
6. **Mathematical Correctness**: Verified with known matrix calculations

---

## Performance Considerations

### Time Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Constructor | O(n×m) | Memory allocation and initialization |
| Destructor | O(1) | Just deallocate pointer |
| Copy Constructor | O(n×m) | Deep copy of all elements |
| Copy Assignment | O(n×m) | Copy data, reallocation if needed |
| Addition | O(n×m) | Element-wise operation |
| Subtraction | O(n×m) | Element-wise operation |
| Multiplication | O(n×p×q) | For n×p × p×q = n×q result |
| Equality | O(n×m) | Element-wise comparison |
| Element Access | O(1) | Direct array indexing |

### Space Complexity

| Operation | Space | Notes |
|-----------|-------|-------|
| Storage | O(n×m) | All n×m elements stored |
| Addition Result | O(n×m) | New matrix created |
| Multiplication Result | O(n×p) | For n×q × q×p result |

### Optimization Tips

1. **Use `operator()` for frequent access** - No bounds checking overhead
2. **Use `at()` only when validation needed** - Bounds checking adds overhead
3. **Avoid unnecessary copies** - Use references when possible
4. **For large matrices, consider using Move semantics** - Currently not implemented
5. **Use `operator[]` for row-based algorithms** - More cache-friendly

### Memory Layout Benefits

Row-major storage provides better **cache locality** for:
- Row-based iterations
- Matrix-vector multiplication
- Row-wise operations

---

## Statistics Tracking

The Matrix class includes built-in statistics for analysis:

```cpp
// Accessing statistics
int instances = Matrix::getInstanceCount();
int copies = Matrix::getCopyCount();
int moves = Matrix::getMoveCount();

// Reset statistics
Matrix::resetStats();

// Print formatted statistics
Matrix::printStats();
```

**Example Output:**
```
========== Matrix Statistics ==========
Active Instances: 15
Total Copies:     7
Total Moves:      0
========================================
```

---

## Files Overview

| File | Purpose |
|------|---------|
| `matrix.h` | Class definition with operator declarations |
| `matrix.cpp` | Implementation of all methods and operators |
| `matrix_demo.cpp` | Comprehensive feature demonstration |
| `operator_demo.cpp` | Focused operator overload showcase |
| `matrix_tests.cpp` | Unit test suite (56 tests) |

---

## Compiler Requirements

- **C++ Standard**: C++17 or later
- **Compiler**: GCC 7+, Clang 5+, MSVC 2017+
- **Headers Required**: `<iostream>`, `<cstring>`, `<stdexcept>`, `<iomanip>`, `<cmath>`

---

## Future Enhancements

Potential improvements for advanced implementations:

1. **Move Semantics**: Implement move constructor and move assignment
2. **Vector Operations**: Scalar multiplication, dot product
3. **Matrix Decomposition**: LU, QR, SVD decompositions
4. **Inverse Calculation**: Matrix inversion
5. **Transpose**: Matrix transposition
6. **Determinant**: Calculate matrix determinant
7. **Template Class**: Generalize for different data types (int, float, complex)
8. **SIMD Optimization**: Use vectorized operations for performance
9. **Sparse Matrix Support**: For matrices with many zeros
10. **Expression Templates**: For efficient chained operations

---

## References

- **C++ Standard Library**: Memory management with `new`/`delete`
- **Operator Overloading**: C++ Reference for syntax and best practices
- **Matrix Mathematics**: Standard linear algebra concepts
- **Memory Safety**: Rule of Five pattern

---

## License & Usage

This Matrix class implementation is provided for educational purposes to demonstrate:
- Operator overloading in C++
- Memory management and the Rule of Five
- Deep vs. shallow copying
- Proper error handling with exceptions
- Statistics tracking for performance analysis

**Feel free to use, modify, and extend for learning purposes.**
