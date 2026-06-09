#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring>
#include <stdexcept>

/**
 * Matrix class for demonstrating operator overloading and efficiency analysis.
 * Manages a 2D matrix stored as a 1D dynamic array in row-major order.
 * 
 * Implements the Rule of Five (Complete):
 * - Constructor
 * - Destructor
 * - Copy Constructor
 * - Copy Assignment Operator
 * - Move Constructor (V2)
 * - Move Assignment Operator (V2)
 */
class Matrix {
private:
    double* data;
    int rows_;
    int cols_;
    
    // Statistics for analyzing object creation/destruction
    static int instance_count;
    static int copy_count;
    static int move_count;

public:
    // ============================================================
    // Constructors and Destructors
    // ============================================================
    
    /**
     * Constructor: Creates a matrix with given dimensions.
     * Initializes all elements to 0.0.
     * 
     * @param rows Number of rows
     * @param cols Number of columns
     */
    Matrix(int rows, int cols);
    
    /**
     * Destructor: Frees allocated memory and updates statistics.
     */
    ~Matrix();
    
    /**
     * Copy Constructor: Creates a deep copy of another matrix.
     * 
     * @param other The matrix to copy
     */
    Matrix(const Matrix& other);
    
    /**
     * Copy Assignment Operator: Assigns the contents of another matrix.
     * Implements proper self-assignment handling and memory management.
     * 
     * @param other The matrix to assign from
     * @return Reference to this matrix
     */
    Matrix& operator=(const Matrix& other);
    
    /**
     * Move Constructor: Transfers ownership of data from a temporary matrix.
     * Efficient transfer without copying data.
     * 
     * @param other The temporary matrix to move from (rvalue reference)
     */
    Matrix(Matrix&& other) noexcept;
    
    /**
     * Move Assignment Operator: Transfers ownership of data from a temporary matrix.
     * Efficient transfer without copying data.
     * 
     * @param other The temporary matrix to move from (rvalue reference)
     * @return Reference to this matrix
     */
    Matrix& operator=(Matrix&& other) noexcept;
    
    // ============================================================
    // Accessors
    // ============================================================
    
    /**
     * @return Number of rows
     */
    int rows() const { return rows_; }
    
    /**
     * @return Number of columns
     */
    int cols() const { return cols_; }
    
    /**
     * Access element at (row, col) - const version.
     * 
     * @param row Row index (0-based)
     * @param col Column index (0-based)
     * @return Constant reference to the element
     * @throws std::out_of_range if indices are out of bounds
     */
    const double& at(int row, int col) const;
    
    /**
     * Access element at (row, col) - non-const version.
     * 
     * @param row Row index (0-based)
     * @param col Column index (0-based)
     * @return Reference to the element
     * @throws std::out_of_range if indices are out of bounds
     */
    double& at(int row, int col);
    
    /**
     * Unchecked element access via operator().
     * 
     * @param row Row index
     * @param col Column index
     * @return Constant reference to the element
     */
    const double& operator()(int row, int col) const;
    
    /**
     * Unchecked element access via operator().
     * 
     * @param row Row index
     * @param col Column index
     * @return Reference to the element
     */
    double& operator()(int row, int col);
    
    // ============================================================
    // Arithmetic Operators (Member Functions - Compound Assignment)
    // ============================================================
    
    /**
     * Compound addition assignment operator (V3: Member function).
     * Adds another matrix element-wise to this matrix.
     * Modifies this matrix in-place.
     * 
     * @param other The matrix to add
     * @return Reference to this matrix
     * @throws std::invalid_argument if dimensions don't match
     */
    Matrix& operator+=(const Matrix& other);
    
    /**
     * Compound subtraction assignment operator (V3: Member function).
     * Subtracts another matrix element-wise from this matrix.
     * Modifies this matrix in-place.
     * 
     * @param other The matrix to subtract
     * @return Reference to this matrix
     * @throws std::invalid_argument if dimensions don't match
     */
    Matrix& operator-=(const Matrix& other);
    
    /**
     * Matrix multiplication operator.
     * Performs matrix multiplication (this * other).
     * 
     * @param other The matrix to multiply with
     * @return New matrix containing the product
     * @throws std::invalid_argument if inner dimensions don't match
     */
    Matrix operator*(const Matrix& other) const;
    
    // Friends for free function operators (V3)
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
    
    // ============================================================
    // Comparison Operators
    // ============================================================
    
    /**
     * Equality comparison operator.
     * Compares two matrices element-wise with tolerance.
     * 
     * @param other The matrix to compare with
     * @return true if matrices are equal, false otherwise
     */
    bool operator==(const Matrix& other) const;
    
    // ============================================================
    // Subscript Operator
    // ============================================================
    
    /**
     * Row access operator.
     * Returns a pointer to the first element of the specified row.
     * 
     * @param row Row index
     * @return Pointer to the row data
     * @throws std::out_of_range if row index is out of bounds
     */
    double* operator[](int row);
    
    /**
     * Row access operator (const version).
     * Returns a const pointer to the first element of the specified row.
     * 
     * @param row Row index
     * @return Const pointer to the row data
     * @throws std::out_of_range if row index is out of bounds
     */
    const double* operator[](int row) const;
    
    // ============================================================
    // Utility Methods
    // ============================================================
    
    /**
     * Fill all elements with a specific value.
     * 
     * @param value The value to fill with
     */
    void fill(double value);
    
    /**
     * Print the matrix to standard output.
     */
    void print(const char* name = nullptr) const;
    
    // ============================================================
    // Statistics (for benchmarking)
    // ============================================================
    
    /**
     * Get the current instance count.
     */
    static int getInstanceCount() { return instance_count; }
    
    /**
     * Get the total number of copies made.
     */
    static int getCopyCount() { return copy_count; }
    
    /**
     * Get the total number of moves made.
     */
    static int getMoveCount() { return move_count; }
    
    /**
     * Reset all statistics counters.
     */
    static void resetStats();
    
    /**
     * Print current statistics.
     */
    static void printStats();

private:
    /**
     * Internal helper to calculate linear index from 2D coordinates.
     * 
     * @param row Row index
     * @param col Column index
     * @return Linear index
     */
    int index(int row, int col) const {
        return row * cols_ + col;
    }
};

// ============================================================
// Stream Insertion Operator (Friend Function)
// ============================================================

/**
 * Stream insertion operator for Matrix.
 * Outputs the matrix in a formatted way.
 * 
 * @param os The output stream
 * @param mat The matrix to output
 * @return Reference to the output stream
 */
std::ostream& operator<<(std::ostream& os, const Matrix& mat);

// ============================================================
// Arithmetic Free Functions (V3: Using Member Functions)
// ============================================================

/**
 * Matrix addition free function (V3).
 * Adds two matrices element-wise using the += member operator.
 * Creates a new matrix without modifying the operands.
 * 
 * @param lhs The left-hand side matrix (first operand)
 * @param rhs The right-hand side matrix (second operand)
 * @return New matrix containing the sum
 * @throws std::invalid_argument if dimensions don't match
 */
Matrix operator+(const Matrix& lhs, const Matrix& rhs);

/**
 * Matrix subtraction free function (V3).
 * Subtracts two matrices element-wise using the -= member operator.
 * Creates a new matrix without modifying the operands.
 * 
 * @param lhs The left-hand side matrix (minuend)
 * @param rhs The right-hand side matrix (subtrahend)
 * @return New matrix containing the difference
 * @throws std::invalid_argument if dimensions don't match
 */
Matrix operator-(const Matrix& lhs, const Matrix& rhs);

#endif // MATRIX_H
