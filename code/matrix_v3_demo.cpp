#include "matrix.h"
#include <iostream>
#include <iomanip>

/**
 * V3: Compound Assignment Operators (+=, -=) and Free Functions
 * 
 * Demonstrates:
 * 1. Member operators: += and -= (in-place modification)
 * 2. Free functions: + and - (using member operators internally)
 * 3. DRY principle: Avoiding code duplication
 * 4. Performance: In-place vs copy-and-modify patterns
 */

int main() {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  V3: Compound Assignment Operators & Free Functions\n";
    std::cout << std::string(70, '=') << "\n";
    
    // ============================================================
    // Demo 1: Compound Addition (+=)
    // ============================================================
    std::cout << "\n--- Demo 1: Compound Addition (+=) ---\n";
    std::cout << "Demonstrates in-place addition using member operator\n\n";
    
    Matrix A(2, 2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 4.0;
    
    Matrix B(2, 2);
    B(0, 0) = 5.0; B(0, 1) = 6.0;
    B(1, 0) = 7.0; B(1, 1) = 8.0;
    
    std::cout << "A = " << std::endl << A << "\n";
    std::cout << "B = " << std::endl << B << "\n";
    
    std::cout << "Performing: A += B;\n";
    A += B;  // In-place: A is modified
    
    std::cout << "After A += B:\n" << A << "\n";
    std::cout << "Note: A is MODIFIED in-place (no copy created)\n";
    
    // ============================================================
    // Demo 2: Compound Subtraction (-=)
    // ============================================================
    std::cout << "\n--- Demo 2: Compound Subtraction (-=) ---\n";
    std::cout << "Demonstrates in-place subtraction using member operator\n\n";
    
    Matrix C(2, 2);
    C(0, 0) = 10.0; C(0, 1) = 20.0;
    C(1, 0) = 30.0; C(1, 1) = 40.0;
    
    Matrix D(2, 2);
    D(0, 0) = 1.0; D(0, 1) = 2.0;
    D(1, 0) = 3.0; D(1, 1) = 4.0;
    
    std::cout << "C = " << std::endl << C << "\n";
    std::cout << "D = " << std::endl << D << "\n";
    
    std::cout << "Performing: C -= D;\n";
    C -= D;  // In-place: C is modified
    
    std::cout << "After C -= D:\n" << C << "\n";
    std::cout << "Note: C is MODIFIED in-place (no copy created)\n";
    
    // ============================================================
    // Demo 3: Free Function Addition (+)
    // ============================================================
    std::cout << "\n--- Demo 3: Free Function Addition (+) ---\n";
    std::cout << "Demonstrates free function using += internally\n\n";
    
    Matrix E(2, 2);
    E(0, 0) = 1.0; E(0, 1) = 2.0;
    E(1, 0) = 3.0; E(1, 1) = 4.0;
    
    Matrix F(2, 2);
    F(0, 0) = 5.0; F(0, 1) = 6.0;
    F(1, 0) = 7.0; F(1, 1) = 8.0;
    
    std::cout << "E = " << std::endl << E << "\n";
    std::cout << "F = " << std::endl << F << "\n";
    
    std::cout << "Performing: Matrix G = E + F;\n";
    Matrix G = E + F;  // Free function: E and F unchanged, new G created
    
    std::cout << "E (original, unchanged):\n" << E;
    std::cout << "F (original, unchanged):\n" << F;
    std::cout << "G = E + F (new matrix):\n" << G << "\n";
    std::cout << "Note: E and F are UNCHANGED\n";
    std::cout << "Implementation: G = E; G += F; (then return via move)\n";
    
    // ============================================================
    // Demo 4: Free Function Subtraction (-)
    // ============================================================
    std::cout << "\n--- Demo 4: Free Function Subtraction (-) ---\n";
    std::cout << "Demonstrates free function using -= internally\n\n";
    
    Matrix H(2, 2);
    H(0, 0) = 10.0; H(0, 1) = 20.0;
    H(1, 0) = 30.0; H(1, 1) = 40.0;
    
    Matrix I(2, 2);
    I(0, 0) = 1.0; I(0, 1) = 2.0;
    I(1, 0) = 3.0; I(1, 1) = 4.0;
    
    std::cout << "H = " << std::endl << H << "\n";
    std::cout << "I = " << std::endl << I << "\n";
    
    std::cout << "Performing: Matrix J = H - I;\n";
    Matrix J = H - I;  // Free function: H and I unchanged, new J created
    
    std::cout << "H (original, unchanged):\n" << H;
    std::cout << "I (original, unchanged):\n" << I;
    std::cout << "J = H - I (new matrix):\n" << J << "\n";
    std::cout << "Note: H and I are UNCHANGED\n";
    std::cout << "Implementation: J = H; J -= I; (then return via move)\n";
    
    // ============================================================
    // Demo 5: Operator Chaining
    // ============================================================
    std::cout << "\n--- Demo 5: Operator Chaining ---\n";
    std::cout << "Demonstrates chaining with compound operators\n\n";
    
    Matrix K(2, 2);
    K.fill(1.0);
    
    Matrix L(2, 2);
    L.fill(2.0);
    
    Matrix M(2, 2);
    M.fill(3.0);
    
    std::cout << "K = "; K.print("K");
    std::cout << "L = "; L.print("L");
    std::cout << "M = "; M.print("M");
    
    std::cout << "\nPerforming: K += L; K += M;  (chained in-place)\n";
    K += L;  // K = K + L
    K += M;  // K = K + M
    std::cout << "K after chained += operations: "; K.print("K");
    
    // ============================================================
    // Demo 6: Complex Expression
    // ============================================================
    std::cout << "\n--- Demo 6: Complex Expression ---\n";
    std::cout << "Demonstrates: (A + B) - C using free functions\n\n";
    
    Matrix P(2, 2);
    P(0, 0) = 1.0; P(0, 1) = 2.0;
    P(1, 0) = 3.0; P(1, 1) = 4.0;
    
    Matrix Q(2, 2);
    Q(0, 0) = 5.0; Q(0, 1) = 6.0;
    Q(1, 0) = 7.0; Q(1, 1) = 8.0;
    
    Matrix R(2, 2);
    R(0, 0) = 1.0; R(0, 1) = 1.0;
    R(1, 0) = 1.0; R(1, 1) = 1.0;
    
    std::cout << "P = " << std::endl << P << "\n";
    std::cout << "Q = " << std::endl << Q << "\n";
    std::cout << "R = " << std::endl << R << "\n";
    
    std::cout << "Performing: Matrix Result = (P + Q) - R;\n";
    Matrix Result = (P + Q) - R;
    
    std::cout << "Result = (P + Q) - R:\n" << Result;
    std::cout << "\nExecution flow:\n";
    std::cout << "  1. temp1 = P + Q;    (free function: copy P, += Q)\n";
    std::cout << "  2. Result = temp1 - R; (free function: copy temp1, -= R)\n";
    std::cout << "  3. With move semantics: No intermediate copies!\n";
    
    // ============================================================
    // Demo 7: Comparison with Traditional Approach
    // ============================================================
    std::cout << "\n--- Demo 7: Implementation Benefits (V3) ---\n";
    std::cout << "Why compound operators and free functions are better:\n\n";
    
    std::cout << "V1 (Old Approach):\n";
    std::cout << "  operator+(A, B):    A + B = new result(A.rows, A.cols); copy A; copy B; add\n";
    std::cout << "  operator-(A, B):    A - B = new result(A.rows, A.cols); copy A; copy B; subtract\n";
    std::cout << "  Result: Code duplication! (same logic in two places)\n\n";
    
    std::cout << "V3 (New Approach - This!):\n";
    std::cout << "  operator+=(A, B):   A += B = modify A in-place, add B element-wise\n";
    std::cout << "  operator-=(A, B):   A -= B = modify A in-place, subtract B element-wise\n";
    std::cout << "  operator+(A, B):    A + B = result = A; result += B; return result\n";
    std::cout << "  operator-(A, B):    A - B = result = A; result -= B; return result\n";
    std::cout << "  Benefits:\n";
    std::cout << "    • DRY Principle: Logic defined once, reused in + and -\n";
    std::cout << "    • Consistency: + and += must agree\n";
    std::cout << "    • Maintenance: Fix bug in += → automatically fixed in +\n";
    std::cout << "    • Performance: In-place when you need it (+=, -=)\n";
    std::cout << "    • Flexibility: Use copy-and-modify when you need new matrix\n";
    
    // ============================================================
    // Summary
    // ============================================================
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  V3 Summary\n";
    std::cout << std::string(70, '=') << "\n\n";
    
    std::cout << "Member Operators (In-place modification):\n";
    std::cout << "  • operator+=  : Modify left operand, add right operand\n";
    std::cout << "  • operator-=  : Modify left operand, subtract right operand\n";
    std::cout << "  • Return *this for chaining\n\n";
    
    std::cout << "Free Functions (Copy-and-modify):\n";
    std::cout << "  • operator+   : Returns new matrix = left + right\n";
    std::cout << "  • operator-   : Returns new matrix = left - right\n";
    std::cout << "  • Implemented using compound operators internally\n\n";
    
    std::cout << "Design Pattern Benefits:\n";
    std::cout << "  ✓ DRY: No code duplication\n";
    std::cout << "  ✓ Consistency: Guaranteed correctness\n";
    std::cout << "  ✓ Efficiency: Choose in-place or copy as needed\n";
    std::cout << "  ✓ Flexibility: Works with move semantics optimization\n";
    std::cout << "  ✓ Maintainability: Changes propagate automatically\n\n";
    
    std::cout << std::string(70, '=') << "\n\n";
    
    return 0;
}
