#ifndef MATRIX_V3_H
#define MATRIX_V3_H

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>

/**
 * MatrixV3: Complete Matrix Class with Move Semantics and Compound Operators
 * 
 * Features:
 * - All of V2 (Copy, Move, Rule of Five)
 * - Compound operators: +=, -=
 * - Element access via operator() and operator[]
 * - Arithmetic operations: +, -, *
 * - Statistics tracking (instance, copy, move counts)
 * 
 * The compound operators eliminate temporary creation and enable
 * optimal performance in accumulation loops.
 */
class MatrixV3 {
public:
    // Constructor
    MatrixV3(int rows, int cols);
    
    // Destructor
    ~MatrixV3();
    
    // Copy Constructor
    MatrixV3(const MatrixV3& other);
    
    // Copy Assignment Operator
    MatrixV3& operator=(const MatrixV3& other);
    
    // Move Constructor
    MatrixV3(MatrixV3&& other) noexcept;
    
    // Move Assignment Operator
    MatrixV3& operator=(MatrixV3&& other) noexcept;
    
    // Element Access
    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;
    
    double* operator[](int row);
    const double* operator[](int row) const;
    
    double& at(int row, int col);
    const double& at(int row, int col) const;
    
    // Arithmetic Operations
    MatrixV3 operator+(const MatrixV3& other) const;
    MatrixV3 operator-(const MatrixV3& other) const;
    MatrixV3 operator*(const MatrixV3& other) const;
    
    // Compound Operators (NEW in V3)
    MatrixV3& operator+=(const MatrixV3& other);
    MatrixV3& operator-=(const MatrixV3& other);
    
    // Comparison
    bool operator==(const MatrixV3& other) const;
    
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
    friend std::ostream& operator<<(std::ostream& os, const MatrixV3& mat);

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

#endif // MATRIX_V3_H
