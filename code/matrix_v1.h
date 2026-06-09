#ifndef MATRIX_V1_H
#define MATRIX_V1_H

#include <iostream>
#include <cstring>
#include <stdexcept>

/**
 * MatrixV1: Basic Matrix Class with Copy Semantics Only
 * 
 * Features:
 * - Constructor and Destructor
 * - Copy Constructor (explicit deep copy)
 * - Copy Assignment Operator (safe assignment)
 * - Element access via operator() and operator[]
 * - Arithmetic operations: +, -, *
 * - Statistics tracking (instance, copy counts)
 * 
 * NOT included:
 * - Move constructor/assignment (C++11)
 * - Compound operators (+=, -=)
 */
class MatrixV1 {
public:
    // Constructor
    MatrixV1(int rows, int cols);
    
    // Destructor
    ~MatrixV1();
    
    // Copy Constructor
    MatrixV1(const MatrixV1& other);
    
    // Copy Assignment Operator
    MatrixV1& operator=(const MatrixV1& other);
    
    // Element Access
    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;
    
    double* operator[](int row);
    const double* operator[](int row) const;
    
    double& at(int row, int col);
    const double& at(int row, int col) const;
    
    // Arithmetic Operations
    MatrixV1 operator+(const MatrixV1& other) const;
    MatrixV1 operator-(const MatrixV1& other) const;
    MatrixV1 operator*(const MatrixV1& other) const;
    
    // Comparison
    bool operator==(const MatrixV1& other) const;
    
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
    friend std::ostream& operator<<(std::ostream& os, const MatrixV1& mat);

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

#endif // MATRIX_V1_H
