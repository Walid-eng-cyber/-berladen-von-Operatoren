#include "matrix.h"
#include <iomanip>
#include <cmath>

// Initialize static members
int Matrix::instance_count = 0;
int Matrix::copy_count = 0;
int Matrix::move_count = 0;

// ============================================================
// Constructor
// ============================================================

Matrix::Matrix(int rows, int cols)
    : rows_(rows), cols_(cols) {
    
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    
    // Allocate memory for rows * cols elements
    int size = rows * cols;
    data = new double[size];
    
    // Initialize all elements to 0.0
    std::memset(data, 0, size * sizeof(double));
    
    instance_count++;
    // Conditional debug output in the Matrix constructor
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::Constructor - " << rows << "x" << cols 
              << " (instances: " << instance_count << ")\n";
    #endif
}

// ============================================================
// Destructor
// ============================================================

Matrix::~Matrix() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    
    instance_count--;

    // Conditional debug output in the Matrix destructor
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::Destructor - " << rows_ << "x" << cols_ 
              << " (instances: " << instance_count << ")\n";
    #endif
}

// ============================================================
// Copy Constructor
// ============================================================

Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
    
    // Allocate new memory
    int size = rows_ * cols_;
    data = new double[size];
    
    // Deep copy of all elements
    std::memcpy(data, other.data, size * sizeof(double));
    
    instance_count++;
    copy_count++;
    
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::CopyConstructor - " << rows_ << "x" << cols_ 
              << " (copies: " << copy_count << ", instances: " << instance_count << ")\n";
    #endif
}

// ============================================================
// Copy Assignment Operator
// ============================================================

Matrix& Matrix::operator=(const Matrix& other) {
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::CopyAssignment - " << rows_ << "x" << cols_ 
              << " <- " << other.rows_ << "x" << other.cols_ << "\n";
    #endif
    
    // Check for self-assignment
    if (this == &other) {
        #ifdef DEBUG_MATRIX
        std::cout << "  (self-assignment detected, skipping)\n";
        #endif
        return *this;
    }
    
    // Check if dimensions match
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        // Free old memory
        delete[] data;
        
        // Allocate new memory with new dimensions
        rows_ = other.rows_;
        cols_ = other.cols_;
        int size = rows_ * cols_;
        data = new double[size];
    }
    
    // Copy all elements
    int size = rows_ * cols_;
    std::memcpy(data, other.data, size * sizeof(double));
    
    copy_count++;
    
    #ifdef DEBUG_MATRIX
    std::cout << "  (copy completed, copies: " << copy_count << ")\n";
    #endif
    
    return *this;
}

// ============================================================
// Move Constructor (Rule of Five - Part 4)
// ============================================================
// Purpose: Efficiently initialize a new Matrix by "stealing" resources
//          from a temporary (rvalue) Matrix object.
//
// How it works:
// 1. Parameter 'other&&' is an rvalue reference - it refers to a temporary
//    or an object explicitly marked with std::move()
// 2. Instead of copying all data (expensive), we just copy the pointers
//    to the dynamic memory - "stealing" ownership
// 3. We nullify the source object so it won't delete memory we now own
//
// Performance benefit:
// - Copy constructor: O(n*m) time, allocates new memory, copies all data
// - Move constructor: O(1) time, just swaps pointers
// - For a 1000x1000 matrix: ~1,000,000 data copies vs 3 pointer copies!
//
// When it's called:
// - Matrix m = std::move(temp_matrix);  // Explicit move
// - Matrix m = function_returning_matrix();  // Implicit (RVO/NRVO)
// - Matrix m = Matrix(100, 100);  // Temporary object
//
Matrix::Matrix(Matrix&& other) noexcept
    : data(other.data), rows_(other.rows_), cols_(other.cols_) {
    
    // Step 1: Copy pointers from source (this is O(1), just pointers)
    // Step 2: Nullify the source object to prevent deletion of our memory
    //         When 'other' is destroyed, it will try to delete nullptr (safe)
    other.data = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
    
    // Update statistics
    instance_count++;  // One more Matrix object exists
    move_count++;      // One more move operation performed
    
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::MoveConstructor - " << rows_ << "x" << cols_ 
              << " (moves: " << move_count << ", instances: " << instance_count << ")\n";
    #endif
}

// ============================================================
// Move Assignment Operator (Rule of Five - Part 5)
// ============================================================
// Purpose: Replace this Matrix's resources with resources "stolen" from
//          a temporary (rvalue) Matrix, without copying any data.
//
// How it works:
// 1. Parameter 'other&&' is an rvalue reference (temporary or std::move)
// 2. Free the memory this Matrix currently owns (cleanup old resources)
// 3. Steal the pointers from 'other' (ownership transfer)
// 4. Nullify 'other' so its destructor won't delete our memory
// 5. Return *this to allow chaining (m1 = m2 = std::move(m3))
//
// Performance benefit:
// - Copy assignment: O(n*m), allocates new memory, copies all data, may reallocate
// - Move assignment: O(1), just swaps pointers and nullifies source
// - Works with different dimensions (changes them, not copying data)
//
// When it's called:
// - existing_matrix = std::move(temporary_matrix);
// - m = function_returning_matrix();  // Implicit move
// - m1 = m2 = std::move(m3);  // Move with chaining (returns *this)
//
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::MoveAssignment - " << rows_ << "x" << cols_ 
              << " <- " << other.rows_ << "x" << other.cols_ << "\n";
    #endif
    
    // Step 1: Self-assignment check
    //         Technically rare with move (usually explicit with std::move)
    //         But still important for safety: m = std::move(m);
    if (this == &other) {
        #ifdef DEBUG_MATRIX
        std::cout << "  (self-assignment detected, skipping)\n";
        #endif
        return *this;  // Avoid deleting memory we're about to take!
    }
    
    // Step 2: Free the old memory this Matrix owns
    //         This prevents memory leaks when replacing old data
    //         Safe even if data is nullptr (delete[] nullptr is safe)
    delete[] data;
    
    // Step 3: Steal ownership - copy pointers from source
    //         This is O(1) - just copy 3 integers/pointers
    //         Old memory (if any) is already deleted above
    data = other.data;
    rows_ = other.rows_;
    cols_ = other.cols_;
    
    // Step 4: Nullify the source object
    //         When 'other' is destroyed, it will delete nullptr (safe)
    //         This prevents double-deletion bugs
    //         Source is now in "moved-from" state: 0x0 empty matrix
    other.data = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
    
    // Update statistics
    move_count++;  // One more move operation performed
    
    #ifdef DEBUG_MATRIX
    std::cout << "  (move completed, moves: " << move_count << ")\n";
    #endif
    
    return *this;  // Allow chaining: m1 = m2 = std::move(m3)
}

// ============================================================
// Element Access
// ============================================================

// Const version of at() - for const Matrix objects (read-only access)
// Called on: const Matrix objects
// Returns: const reference (prevents modification)
// Example: const Matrix cm; double x = cm.at(0, 0);  // ✓ OK
//          const Matrix cm; cm.at(0, 0) = 5.0;      // ✗ ERROR
const double& Matrix::at(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[index(row, col)];
}

// Non-const version of at() - for non-const Matrix objects (read/write access)
// Called on: non-const Matrix objects
// Returns: non-const reference (allows modification)
// Example: Matrix m; m.at(0, 0) = 5.0;          // ✓ OK (write)
//          Matrix m; double x = m.at(0, 0);     // ✓ OK (read)
// WHY TWO VERSIONS? Const-correctness:
// - If we only had the non-const version, const matrices couldn't call at() at all
// - The compiler picks the right version based on whether the object is const
double& Matrix::at(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[index(row, col)];
}

// Const version of operator() - for const Matrix objects (read-only access)
// Called on: const Matrix objects
// Returns: const reference (prevents modification)
// Example: const Matrix cm; double x = cm(0, 0);  // ✓ OK
//          const Matrix cm; cm(0, 0) = 5.0;      // ✗ ERROR
const double& Matrix::operator()(int row, int col) const {
    return data[index(row, col)];
}

// Non-const version of operator() - for non-const Matrix objects (read/write access)
// Called on: non-const Matrix objects
// Returns: non-const reference (allows modification)
// Example: Matrix m; m(0, 0) = 5.0;          // ✓ OK (write)
//          Matrix m; double x = m(0, 0);     // ✓ OK (read)
// NOTE: Unlike at(), this version doesn't do bounds checking - for performance
// WHY TWO VERSIONS? Same reason as at():
// - Const version allows const matrices to read (but not write)
// - Non-const version allows non-const matrices to read AND write
// - Compiler automatically picks the right version based on const-ness of the object
double& Matrix::operator()(int row, int col) {
    return data[index(row, col)];
}

// ============================================================
// Utility Methods
// ============================================================

void Matrix::fill(double value) {
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] = value;
    }
}

void Matrix::print(const char* name) const {
    if (name != nullptr) {
        std::cout << "Matrix " << name << " (" << rows_ << "x" << cols_ << "):\n";
    } else {
        std::cout << "Matrix (" << rows_ << "x" << cols_ << "):\n";
    }
    
    for (int i = 0; i < rows_; ++i) {
        std::cout << "  [";
        for (int j = 0; j < cols_; ++j) {
            printf("%8.2f", data[index(i, j)]);
            if (j < cols_ - 1) std::cout << " ";
        }
        std::cout << " ]\n";
    }
}

// ============================================================
// Statistics
// ============================================================

void Matrix::resetStats() {
    instance_count = 0;
    copy_count = 0;
    move_count = 0;
}

void Matrix::printStats() {
    std::cout << "\n========== Matrix Statistics ==========\n";
    std::cout << "Active Instances: " << instance_count << "\n";
    std::cout << "Total Copies:     " << copy_count << "\n";
    std::cout << "Total Moves:      " << move_count << "\n";
    std::cout << "========================================\n";
}

// ============================================================
// Compound Assignment Operators (V3: Member Functions)
// ============================================================

Matrix& Matrix::operator+=(const Matrix& other) {
    // Dimension validation
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    // Add element-wise, modifying this matrix in-place
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] += other.data[i];  // Compound operation: add to existing elements
    }
    
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::operator+= - " << rows_ << "x" << cols_ << " modified in-place\n";
    #endif
    
    return *this;  // Return reference for chaining: (A += B) += C
}

Matrix& Matrix::operator-=(const Matrix& other) {
    // Dimension validation
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    // Subtract element-wise, modifying this matrix in-place
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] -= other.data[i];  // Compound operation: subtract from existing elements
    }
    
    #ifdef DEBUG_MATRIX
    std::cout << "Matrix::operator-= - " << rows_ << "x" << cols_ << " modified in-place\n";
    #endif
    
    return *this;  // Return reference for chaining: (A -= B) -= C
}

// ============================================================
// Multiplication Operator
// ============================================================

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Inner dimensions must match for matrix multiplication");
    }
    
    Matrix result(rows_, other.cols_);
    
    // Standard matrix multiplication: result(i,j) = sum(this(i,k) * other(k,j))
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

// ============================================================
// Comparison Operators
// ============================================================

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

// ============================================================
// Subscript Operator
// ============================================================

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

// ============================================================
// Stream Insertion Operator
// ============================================================

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

// ============================================================
// Arithmetic Free Functions (V3: Using Member Operators)
// ============================================================

/**
 * Free function: Matrix addition using += member operator.
 * Creates a copy of lhs, adds rhs to it, and returns the result.
 * The original matrices are not modified.
 * 
 * Implementation technique (V3):
 * - Create a copy of the left operand (copy constructor)
 * - Use the += member operator to add rhs to the copy
 * - Return the modified copy (move semantics avoids another copy)
 * 
 * Benefits:
 * - DRY principle: Addition logic defined once in += operator
 * - Consistent behavior: operator+ and operator+= use same algorithm
 * - Enables chaining: (A + B) + C evaluates left-to-right
 * 
 * Expression: result = lhs + rhs
 * Equivalent to: result = lhs; result += rhs;
 */
Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;      // Copy constructor: copy left operand
    result += rhs;            // Member operator +=: add right operand
    return result;            // Return by value (move semantics optimizes this)
}

/**
 * Free function: Matrix subtraction using -= member operator.
 * Creates a copy of lhs, subtracts rhs from it, and returns the result.
 * The original matrices are not modified.
 * 
 * Implementation technique (V3):
 * - Create a copy of the left operand (copy constructor)
 * - Use the -= member operator to subtract rhs from the copy
 * - Return the modified copy (move semantics avoids another copy)
 * 
 * Benefits:
 * - DRY principle: Subtraction logic defined once in -= operator
 * - Consistent behavior: operator- and operator-= use same algorithm
 * - Enables chaining: (A - B) - C evaluates left-to-right
 * 
 * Expression: result = lhs - rhs
 * Equivalent to: result = lhs; result -= rhs;
 */
Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;      // Copy constructor: copy left operand
    result -= rhs;            // Member operator -=: subtract right operand
    return result;            // Return by value (move semantics optimizes this)
}


