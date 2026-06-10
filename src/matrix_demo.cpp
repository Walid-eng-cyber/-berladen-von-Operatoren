#include "matrix.h"
#include <iostream>
#include <iomanip>

/**
 * Matrix Demo Program
 * Demonstrates copy constructor, copy assignment, and statistics tracking.
 */

int main() {
    std::cout << "========== Matrix Class Demo ==========\n\n";
    
    // Reset statistics before starting
    Matrix::resetStats();
    
    // ============================================================
    // Demo 1: Constructor
    // ============================================================
    std::cout << "--- Demo 1: Constructor ---\n";
    Matrix A(3, 3);
    A.fill(1.0);
    std::cout << "Created matrix A (3x3):\n";
    A.print("A");
    std::cout << "\n";
    
    Matrix B(2, 4);
    B.fill(2.0);
    std::cout << "Created matrix B (2x4):\n";
    B.print("B");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 2: Copy Constructor
    // ============================================================
    std::cout << "\n--- Demo 2: Copy Constructor ---\n";
    std::cout << "Creating copy of A: Matrix C = A;\n";
    Matrix C = A;  // Copy constructor called
    std::cout << "\nMatrix C (copy of A):\n";
    C.print("C");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 3: Copy Assignment - Different Dimensions
    // ============================================================
    std::cout << "\n--- Demo 3: Copy Assignment (different dimensions) ---\n";
    std::cout << "Assigning B to C: C = B;\n";
    std::cout << "C was 3x3, B is 2x4 (requires reallocation)\n";
    C = B;  // Copy assignment with dimension change
    std::cout << "\nMatrix C (after assignment from B):\n";
    C.print("C");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 4: Copy Assignment - Same Dimensions
    // ============================================================
    std::cout << "\n--- Demo 4: Copy Assignment (same dimensions) ---\n";
    Matrix D(2, 4);
    D.fill(3.0);
    std::cout << "Created matrix D (2x4):\n";
    D.print("D");
    
    std::cout << "\nAssigning B to D: D = B;\n";
    std::cout << "D and B have same dimensions (no reallocation needed)\n";
    D = B;  // Copy assignment without reallocation
    std::cout << "\nMatrix D (after assignment from B):\n";
    D.print("D");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 5: Self-Assignment (edge case)
    // ============================================================
    std::cout << "\n--- Demo 5: Self-Assignment (edge case) ---\n";
    std::cout << "Self-assigning: B = B;\n";
    B = B;  // Self-assignment (should be detected)
    std::cout << "Self-assignment detected and handled correctly.\n";
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 6: Element Access
    // ============================================================
    std::cout << "\n--- Demo 6: Element Access ---\n";
    std::cout << "Setting elements in matrix E:\n";
    Matrix E(2, 3);
    
    E(0, 0) = 1.1;
    E(0, 1) = 1.2;
    E(0, 2) = 1.3;
    E(1, 0) = 2.1;
    E(1, 1) = 2.2;
    E(1, 2) = 2.3;
    
    E.print("E");
    
    std::cout << "\nAccessing elements:\n";
    std::cout << "E(0,0) = " << E(0, 0) << "\n";
    std::cout << "E(1,2) = " << E(1, 2) << "\n";
    
    std::cout << "\nUsing at() with bounds checking:\n";
    try {
        std::cout << "E.at(0,0) = " << E.at(0, 0) << "\n";
        std::cout << "E.at(1,2) = " << E.at(1, 2) << "\n";
        // Uncomment to test bounds checking:
        // std::cout << "E.at(2,0) = " << E.at(2, 0) << "\n";  // Should throw
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 7: Multiple Copies (Chain)
    // ============================================================
    std::cout << "\n--- Demo 7: Multiple Copies (chain) ---\n";
    Matrix F = A;
    Matrix G = F;
    Matrix H = G;
    std::cout << "Created chain of copies: F = A; G = F; H = G;\n";
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 8: Operator+ (Matrix Addition)
    // ============================================================
    std::cout << "\n--- Demo 8: Operator+ (Matrix Addition) ---\n";
    Matrix M1(2, 2);
    M1(0, 0) = 1.0; M1(0, 1) = 2.0;
    M1(1, 0) = 3.0; M1(1, 1) = 4.0;
    std::cout << "Matrix M1 (2x2):\n";
    M1.print("M1");
    std::cout << "\n";
    
    Matrix M2(2, 2);
    M2(0, 0) = 5.0; M2(0, 1) = 6.0;
    M2(1, 0) = 7.0; M2(1, 1) = 8.0;
    std::cout << "Matrix M2 (2x2):\n";
    M2.print("M2");
    std::cout << "\n";
    
    Matrix M3 = M1 + M2;
    std::cout << "Result M3 = M1 + M2:\n";
    M3.print("M3");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 9: Operator- (Matrix Subtraction)
    // ============================================================
    std::cout << "\n--- Demo 9: Operator- (Matrix Subtraction) ---\n";
    Matrix M4 = M3 - M1;
    std::cout << "Result M4 = M3 - M1:\n";
    M4.print("M4");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 10: Operator* (Matrix Multiplication)
    // ============================================================
    std::cout << "\n--- Demo 10: Operator* (Matrix Multiplication) ---\n";
    Matrix M5(2, 3);
    M5(0, 0) = 1.0; M5(0, 1) = 2.0; M5(0, 2) = 3.0;
    M5(1, 0) = 4.0; M5(1, 1) = 5.0; M5(1, 2) = 6.0;
    std::cout << "Matrix M5 (2x3):\n";
    M5.print("M5");
    std::cout << "\n";
    
    Matrix M6(3, 2);
    M6(0, 0) = 7.0; M6(0, 1) = 8.0;
    M6(1, 0) = 9.0; M6(1, 1) = 10.0;
    M6(2, 0) = 11.0; M6(2, 1) = 12.0;
    std::cout << "Matrix M6 (3x2):\n";
    M6.print("M6");
    std::cout << "\n";
    
    Matrix M7 = M5 * M6;  // 2x3 * 3x2 = 2x2
    std::cout << "Result M7 = M5 * M6 (2x2):\n";
    M7.print("M7");
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 11: Operator== (Equality Comparison)
    // ============================================================
    std::cout << "\n--- Demo 11: Operator== (Equality Comparison) ---\n";
    Matrix M8(2, 2);
    M8.fill(5.0);
    Matrix M9 = M8;
    std::cout << "M8 and M9 are " << (M8 == M9 ? "EQUAL" : "NOT EQUAL") << "\n";
    std::cout << "M1 and M2 are " << (M1 == M2 ? "EQUAL" : "NOT EQUAL") << "\n";
    std::cout << "M1 and M4 are " << (M1 == M4 ? "EQUAL" : "NOT EQUAL") << "\n\n";
    
    // ============================================================
    // Demo 12: Operator[] (Row Access)
    // ============================================================
    std::cout << "--- Demo 12: Operator[] (Row Access) ---\n";
    Matrix M10(3, 4);
    M10(0, 0) = 1.1; M10(0, 1) = 1.2; M10(0, 2) = 1.3; M10(0, 3) = 1.4;
    M10(1, 0) = 2.1; M10(1, 1) = 2.2; M10(1, 2) = 2.3; M10(1, 3) = 2.4;
    M10(2, 0) = 3.1; M10(2, 1) = 3.2; M10(2, 2) = 3.3; M10(2, 3) = 3.4;
    
    std::cout << "Matrix M10 (3x4):\n";
    M10.print("M10");
    std::cout << "\n";
    
    std::cout << "Accessing row 1 with operator[]:\n";
    double* row = M10[1];
    std::cout << "M10[1]: [ ";
    for (int i = 0; i < M10.cols(); ++i) {
        std::cout << row[i] << " ";
    }
    std::cout << "]\n\n";
    
    // ============================================================
    // Demo 13: Operator<< (Stream Insertion)
    // ============================================================
    std::cout << "--- Demo 13: Operator<< (Stream Insertion) ---\n";
    std::cout << "Using operator<< for formatted output:\n\n";
    std::cout << M1;
    std::cout << "\n";
    std::cout << M5;
    std::cout << "\n";
    
    Matrix::printStats();
    
    std::cout << "All matrices will be destroyed when they go out of scope.\n";
    std::cout << "Final statistics after destruction:\n";
    
    return 0;
    // Destructors called here for all matrices
}
