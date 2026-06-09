#include "matrix.h"
#include <iostream>
#include <iomanip>

/**
 * Operator Overloading Demo
 * Demonstrates +, -, *, ==, [], and << operators
 */

int main() {
    std::cout << "========== Operator Overloading Demo ==========\n\n";
    
    Matrix::resetStats();
    
    // ============================================================
    // Demo 1: Operator+ (Matrix Addition)
    // ============================================================
    std::cout << "--- Demo 1: Operator+ (Matrix Addition) ---\n";
    Matrix A(2, 3);
    A.fill(1.0);
    std::cout << "Matrix A (filled with 1.0):\n" << A << "\n";
    
    Matrix B(2, 3);
    B.fill(2.0);
    std::cout << "Matrix B (filled with 2.0):\n" << B << "\n";
    
    Matrix C = A + B;
    std::cout << "Result C = A + B:\n" << C << "\n";
    
    // ============================================================
    // Demo 2: Operator- (Matrix Subtraction)
    // ============================================================
    std::cout << "--- Demo 2: Operator- (Matrix Subtraction) ---\n";
    Matrix D = C - A;
    std::cout << "Result D = C - A:\n" << D << "\n";
    
    // ============================================================
    // Demo 3: Operator* (Matrix Multiplication)
    // ============================================================
    std::cout << "--- Demo 3: Operator* (Matrix Multiplication) ---\n";
    Matrix E(2, 3);
    E(0, 0) = 1.0; E(0, 1) = 2.0; E(0, 2) = 3.0;
    E(1, 0) = 4.0; E(1, 1) = 5.0; E(1, 2) = 6.0;
    std::cout << "Matrix E (2x3):\n" << E << "\n";
    
    Matrix F(3, 2);
    F(0, 0) = 7.0;  F(0, 1) = 8.0;
    F(1, 0) = 9.0;  F(1, 1) = 10.0;
    F(2, 0) = 11.0; F(2, 1) = 12.0;
    std::cout << "Matrix F (3x2):\n" << F << "\n";
    
    Matrix G = E * F;  // 2x3 * 3x2 = 2x2
    std::cout << "Result G = E * F (2x2):\n" << G << "\n";
    
    // ============================================================
    // Demo 4: Operator== (Equality Comparison)
    // ============================================================
    std::cout << "--- Demo 4: Operator== (Equality Comparison) ---\n";
    Matrix H(2, 3);
    H.fill(2.0);
    Matrix I = H;
    
    std::cout << "H and I are " << (H == I ? "EQUAL" : "NOT EQUAL") << "\n";
    std::cout << "H and B are " << (H == B ? "EQUAL" : "NOT EQUAL") << "\n";
    std::cout << "A and C are " << (A == C ? "EQUAL" : "NOT EQUAL") << "\n\n";
    
    // ============================================================
    // Demo 5: Operator[] (Row Access)
    // ============================================================
    std::cout << "--- Demo 5: Operator[] (Row Access) ---\n";
    Matrix J(3, 4);
    J(0, 0) = 1.1; J(0, 1) = 1.2; J(0, 2) = 1.3; J(0, 3) = 1.4;
    J(1, 0) = 2.1; J(1, 1) = 2.2; J(1, 2) = 2.3; J(1, 3) = 2.4;
    J(2, 0) = 3.1; J(2, 1) = 3.2; J(2, 2) = 3.3; J(2, 3) = 3.4;
    
    std::cout << "Matrix J:\n" << J << "\n";
    
    std::cout << "Accessing row 1 with operator[]:\n";
    double* row = J[1];
    std::cout << "Row 1: [ ";
    for (int i = 0; i < J.cols(); ++i) {
        std::cout << std::fixed << std::setw(4) << std::setprecision(1) 
                  << row[i] << " ";
    }
    std::cout << "]\n\n";
    
    // ============================================================
    // Demo 6: Operator<< (Stream Output)
    // ============================================================
    std::cout << "--- Demo 6: Operator<< (Stream Output) ---\n";
    std::cout << "Using operator<< to display matrices:\n\n";
    std::cout << A;
    std::cout << "\n";
    std::cout << E;
    std::cout << "\n";
    
    // ============================================================
    // Demo 7: Complex Expression
    // ============================================================
    std::cout << "--- Demo 7: Complex Expression ---\n";
    std::cout << "(A + B) - A should equal B:\n";
    Matrix Complex = (A + B) - A;
    std::cout << Complex;
    std::cout << "Are they equal? " << (Complex == B ? "YES" : "NO") << "\n\n";
    
    // ============================================================
    // Statistics
    // ============================================================
    std::cout << "--- Final Statistics ---\n";
    Matrix::printStats();
    
    return 0;
}
