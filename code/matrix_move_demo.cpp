#include "matrix.h"
#include <iostream>
#include <iomanip>

/**
 * Move Semantics Demo (V2)
 * Demonstrates the difference between copy and move semantics
 */

int main() {
    std::cout << "========== Move Semantics Demo (V2) ==========\n\n";
    
    Matrix::resetStats();
    
    // ============================================================
    // Demo 1: Copy Semantics
    // ============================================================
    std::cout << "--- Demo 1: Copy Semantics (Expensive) ---\n";
    std::cout << "Creating matrix A and assigning to B (copy):\n\n";
    
    Matrix A(2, 2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 4.0;
    std::cout << "Matrix A:\n" << A << "\n";
    
    std::cout << "Performing: Matrix B = A;  (copy constructor)\n";
    Matrix B = A;  // Copy constructor - expensive!
    std::cout << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 2: Move Semantics with Temporary
    // ============================================================
    std::cout << "\n--- Demo 2: Move Semantics (Efficient) ---\n";
    std::cout << "Creating matrix from temporary:\n\n";
    
    std::cout << "Performing: Matrix C = createMatrix();\n";
    
    auto createMatrix = []() {
        Matrix temp(3, 3);
        temp.fill(5.0);
        return temp;  // Move constructor called here!
    };
    
    Matrix C = createMatrix();  // Move constructor - efficient!
    std::cout << "Matrix C:\n" << C << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 3: Return Value Optimization (RVO)
    // ============================================================
    std::cout << "--- Demo 3: Function Return (Move Semantics) ---\n";
    std::cout << "Creating matrix from function return:\n\n";
    
    auto makeMatrix = []() {
        Matrix result(2, 3);
        result(0, 0) = 1.1; result(0, 1) = 2.2; result(0, 2) = 3.3;
        result(1, 0) = 4.4; result(1, 1) = 5.5; result(1, 2) = 6.6;
        return result;  // Move semantics or RVO applied
    };
    
    std::cout << "Performing: Matrix D = makeMatrix();\n";
    Matrix D = makeMatrix();
    std::cout << "Matrix D:\n" << D << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 4: Move Assignment
    // ============================================================
    std::cout << "--- Demo 4: Move Assignment ---\n";
    std::cout << "Assigning temporary matrix:\n\n";
    
    auto createTempMatrix = []() {
        Matrix temp(2, 2);
        temp(0, 0) = 7.0; temp(0, 1) = 8.0;
        temp(1, 0) = 9.0; temp(1, 1) = 10.0;
        return temp;
    };
    
    Matrix E(1, 1);
    E(0, 0) = 0.0;
    std::cout << "Matrix E (before assignment): 1x1\n" << E << "\n";
    
    std::cout << "Performing: E = createTempMatrix(); (move assignment)\n";
    E = createTempMatrix();
    std::cout << "Matrix E (after assignment): 2x2\n" << E << "\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 5: Comparison - Copy vs Move
    // ============================================================
    std::cout << "--- Demo 5: Copy vs Move Comparison ---\n";
    std::cout << "Resetting statistics...\n\n";
    
    Matrix::resetStats();
    
    std::cout << "Creating 5 matrices by COPY:\n";
    Matrix M1(100, 100);
    M1.fill(1.0);
    
    Matrix M2 = M1;  // Copy
    Matrix M3 = M1;  // Copy
    Matrix M4 = M1;  // Copy
    Matrix M5 = M1;  // Copy
    
    std::cout << "After 4 copy operations:\n";
    Matrix::printStats();
    
    std::cout << "\n---\n\n";
    std::cout << "Resetting statistics...\n";
    Matrix::resetStats();
    
    std::cout << "Creating 5 matrices by MOVE (simulated with temporaries):\n";
    
    auto makeTemp = [](int size) {
        Matrix t(size, size);
        t.fill(1.0);
        return t;
    };
    
    Matrix N1 = makeTemp(100);  // Move
    Matrix N2 = makeTemp(100);  // Move
    Matrix N3 = makeTemp(100);  // Move
    Matrix N4 = makeTemp(100);  // Move
    Matrix N5 = makeTemp(100);  // Move
    
    std::cout << "After 5 move operations:\n";
    Matrix::printStats();
    
    // ============================================================
    // Demo 6: Explicit std::move
    // ============================================================
    std::cout << "\n--- Demo 6: Explicit std::move ---\n";
    std::cout << "Using std::move to explicitly invoke move semantics:\n\n";
    
    std::cout << "Resetting statistics...\n";
    Matrix::resetStats();
    
    Matrix X(2, 2);
    X.fill(3.0);
    std::cout << "Original matrix X created\n";
    Matrix::printStats();
    
    std::cout << "\nPerforming: Matrix Y = std::move(X);\n";
    Matrix Y = std::move(X);  // Explicit move
    std::cout << "After move:\n";
    std::cout << "Matrix Y:\n" << Y << "\n";
    std::cout << "Matrix X is now in moved-from state (empty)\n";
    std::cout << "Matrix X dimensions: " << X.rows() << "x" << X.cols() << "\n\n";
    
    Matrix::printStats();
    
    // ============================================================
    // Demo 7: Move Semantics in Chained Operations
    // ============================================================
    std::cout << "\n--- Demo 7: Move Semantics in Operations ---\n";
    std::cout << "Chained operations benefit from move semantics:\n\n";
    
    std::cout << "Resetting statistics...\n";
    Matrix::resetStats();
    
    Matrix P(2, 2);
    P(0, 0) = 1.0; P(0, 1) = 2.0;
    P(1, 0) = 3.0; P(1, 1) = 4.0;
    
    Matrix Q(2, 2);
    Q(0, 0) = 5.0; Q(0, 1) = 6.0;
    Q(1, 0) = 7.0; Q(1, 1) = 8.0;
    
    std::cout << "Performing: Matrix R = (P + Q);\n";
    std::cout << "The temporary from (P + Q) is moved to R\n";
    Matrix R = (P + Q);  // Temporary from P + Q is moved!
    
    std::cout << "\nResult Matrix R:\n" << R << "\n";
    Matrix::printStats();
    
    // ============================================================
    // Summary Statistics
    // ============================================================
    std::cout << "\n--- Summary ---\n";
    std::cout << "Move semantics is crucial for:\n";
    std::cout << "  • Function return values\n";
    std::cout << "  • Temporary objects\n";
    std::cout << "  • std::move() explicit moves\n";
    std::cout << "  • Large matrices (avoids copying huge data)\n";
    std::cout << "\nBenefit: Avoids expensive deep copies of large matrices!\n";
    
    return 0;
}
