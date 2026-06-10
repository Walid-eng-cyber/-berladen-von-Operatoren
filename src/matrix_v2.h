#ifndef MATRIX_V2_H
#define MATRIX_V2_H

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>

/**
 * MatrixV2: Matrix Class with Copy and Move Semantics
 * 
 * Features:
 * - All of V1 (Constructor, Destructor, Copy operations)
 * - Move Constructor (pointer theft, O(1))
 * - Move Assignment Operator (efficient ownership transfer)
 * - Element access via operator() and operator[]
 * - Arithmetic operations: +, -, *
 * - Statistics tracking (instance, copy, move counts)
 * 
 * NOT included:
 * - Compound operators (+=, -=)
 */
class MatrixV2 {
public:
    // Constructor
    MatrixV2(int rows, int cols);
    
    // Destructor
    ~MatrixV2();
    
    // Copy Constructor
    MatrixV2(const MatrixV2& other);
    
    // Copy Assignment Operator
    MatrixV2& operator=(const MatrixV2& other);
    
    // Move Constructor (NEW in V2)
    MatrixV2(MatrixV2&& other) noexcept;
    
    // Move Assignment Operator (NEW in V2)
    MatrixV2& operator=(MatrixV2&& other) noexcept;
    
    // Element Access
    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;
    
    double* operator[](int row);
    const double* operator[](int row) const;
    
    double& at(int row, int col);
    const double& at(int row, int col) const;
    
    // Arithmetic Operations
    MatrixV2 operator+(const MatrixV2& other) const;
    MatrixV2 operator-(const MatrixV2& other) const;
    MatrixV2 operator*(const MatrixV2& other) const;
    
    // Comparison
    bool operator==(const MatrixV2& other) const;
    
    // Utility
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    
    void fill(double value);
    
    // Statistics
    static void resetStats();
    static void printStats();
    static int getInstanceCount();
    static int getCopyCount();
    static int getMoveCount();
    
    // Stream Output
    friend std::ostream& operator<<(std::ostream& os, const MatrixV2& mat);

private:
    double* data;
    int rows_;
    int cols_;
    
    // Statistics
    static int instance_count;
    static int copy_count;
    static int move_count;
    
    // Helper for index calculation
    inline int index(int row, int col) const {
        return row * cols_ + col;
    }
};

#endif // MATRIX_V2_H
