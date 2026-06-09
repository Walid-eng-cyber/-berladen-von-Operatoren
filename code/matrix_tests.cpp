#include "matrix.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>

/**
 * Basic Unit Tests for Matrix Class
 * Tests constructor, copy operations, and all operator overloads
 */

// Test counter
int test_count = 0;
int passed_count = 0;
int failed_count = 0;

// Macro for simple assertions
#define ASSERT(condition, message) \
    test_count++; \
    if (condition) { \
        passed_count++; \
        std::cout << "  ✓ Test " << test_count << " PASSED: " << message << "\n"; \
    } else { \
        failed_count++; \
        std::cout << "  ✗ Test " << test_count << " FAILED: " << message << "\n"; \
    }

// Helper function for floating-point comparison
bool approxEqual(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) < epsilon;
}

// ============================================================
// Test Suite 1: Constructor and Accessors
// ============================================================
void testConstructor() {
    std::cout << "\n========== Test Suite 1: Constructor and Accessors ==========\n";
    
    // Test 1: Basic construction
    Matrix m(3, 4);
    ASSERT(m.rows() == 3, "Constructor sets rows correctly");
    ASSERT(m.cols() == 4, "Constructor sets cols correctly");
    
    // Test 2: Elements initialized to zero
    bool allZero = true;
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            if (m(i, j) != 0.0) {
                allZero = false;
                break;
            }
        }
    }
    ASSERT(allZero, "Constructor initializes all elements to 0");
    
    // Test 3: Fill operation
    m.fill(5.5);
    bool allFilled = true;
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            if (!approxEqual(m(i, j), 5.5)) {
                allFilled = false;
                break;
            }
        }
    }
    ASSERT(allFilled, "Fill operation sets all elements correctly");
}

// ============================================================
// Test Suite 2: Copy Operations
// ============================================================
void testCopyOperations() {
    std::cout << "\n========== Test Suite 2: Copy Operations ==========\n";
    
    // Setup
    Matrix original(2, 3);
    original(0, 0) = 1.1;
    original(0, 1) = 2.2;
    original(0, 2) = 3.3;
    original(1, 0) = 4.4;
    original(1, 1) = 5.5;
    original(1, 2) = 6.6;
    
    // Test 1: Copy constructor
    Matrix copy1(original);
    ASSERT(copy1.rows() == original.rows(), "Copy constructor copies rows");
    ASSERT(copy1.cols() == original.cols(), "Copy constructor copies cols");
    ASSERT(approxEqual(copy1(0, 0), 1.1), "Copy constructor performs deep copy");
    ASSERT(approxEqual(copy1(1, 2), 6.6), "Copy constructor deep copies all elements");
    
    // Test 2: Verify it's a deep copy (modifying copy doesn't affect original)
    copy1(0, 0) = 99.9;
    ASSERT(!approxEqual(original(0, 0), 99.9), "Deep copy: modifying copy doesn't affect original");
    ASSERT(approxEqual(original(0, 0), 1.1), "Original remains unchanged");
    
    // Test 3: Copy assignment (same dimensions)
    Matrix copy2(2, 3);
    copy2.fill(0.0);
    copy2 = original;
    ASSERT(approxEqual(copy2(0, 0), 1.1), "Copy assignment copies data correctly");
    ASSERT(approxEqual(copy2(1, 2), 6.6), "Copy assignment copies all elements");
    
    // Test 4: Copy assignment (different dimensions)
    Matrix smaller(1, 2);
    smaller = original;
    ASSERT(smaller.rows() == 2, "Copy assignment handles dimension change (rows)");
    ASSERT(smaller.cols() == 3, "Copy assignment handles dimension change (cols)");
    ASSERT(approxEqual(smaller(0, 0), 1.1), "Copy assignment data after dimension change");
    
    // Test 5: Self-assignment
    original = original;  // Should not crash
    ASSERT(approxEqual(original(0, 0), 1.1), "Self-assignment preserves data");
}

// ============================================================
// Test Suite 3: Operator+
// ============================================================
void testAddition() {
    std::cout << "\n========== Test Suite 3: Operator+ (Addition) ==========\n";
    
    Matrix a(2, 2);
    a(0, 0) = 1.0; a(0, 1) = 2.0;
    a(1, 0) = 3.0; a(1, 1) = 4.0;
    
    Matrix b(2, 2);
    b(0, 0) = 5.0; b(0, 1) = 6.0;
    b(1, 0) = 7.0; b(1, 1) = 8.0;
    
    Matrix c = a + b;
    
    ASSERT(c.rows() == 2, "Addition result has correct rows");
    ASSERT(c.cols() == 2, "Addition result has correct cols");
    ASSERT(approxEqual(c(0, 0), 6.0), "Addition computes (0,0) correctly");
    ASSERT(approxEqual(c(0, 1), 8.0), "Addition computes (0,1) correctly");
    ASSERT(approxEqual(c(1, 0), 10.0), "Addition computes (1,0) correctly");
    ASSERT(approxEqual(c(1, 1), 12.0), "Addition computes (1,1) correctly");
    
    // Test dimension mismatch
    Matrix d(3, 3);
    try {
        Matrix invalid = a + d;
        ASSERT(false, "Addition throws on dimension mismatch");
    } catch (const std::invalid_argument&) {
        ASSERT(true, "Addition throws on dimension mismatch");
    }
}

// ============================================================
// Test Suite 4: Operator-
// ============================================================
void testSubtraction() {
    std::cout << "\n========== Test Suite 4: Operator- (Subtraction) ==========\n";
    
    Matrix a(2, 2);
    a(0, 0) = 10.0; a(0, 1) = 20.0;
    a(1, 0) = 30.0; a(1, 1) = 40.0;
    
    Matrix b(2, 2);
    b(0, 0) = 1.0; b(0, 1) = 2.0;
    b(1, 0) = 3.0; b(1, 1) = 4.0;
    
    Matrix c = a - b;
    
    ASSERT(c.rows() == 2, "Subtraction result has correct rows");
    ASSERT(c.cols() == 2, "Subtraction result has correct cols");
    ASSERT(approxEqual(c(0, 0), 9.0), "Subtraction computes (0,0) correctly");
    ASSERT(approxEqual(c(0, 1), 18.0), "Subtraction computes (0,1) correctly");
    ASSERT(approxEqual(c(1, 0), 27.0), "Subtraction computes (1,0) correctly");
    ASSERT(approxEqual(c(1, 1), 36.0), "Subtraction computes (1,1) correctly");
}

// ============================================================
// Test Suite 5: Operator* (Multiplication)
// ============================================================
void testMultiplication() {
    std::cout << "\n========== Test Suite 5: Operator* (Multiplication) ==========\n";
    
    // Test 1: 2x3 * 3x2
    Matrix a(2, 3);
    a(0, 0) = 1.0; a(0, 1) = 2.0; a(0, 2) = 3.0;
    a(1, 0) = 4.0; a(1, 1) = 5.0; a(1, 2) = 6.0;
    
    Matrix b(3, 2);
    b(0, 0) = 7.0; b(0, 1) = 8.0;
    b(1, 0) = 9.0; b(1, 1) = 10.0;
    b(2, 0) = 11.0; b(2, 1) = 12.0;
    
    Matrix c = a * b;  // Should be 2x2
    
    ASSERT(c.rows() == 2, "Multiplication result has correct rows");
    ASSERT(c.cols() == 2, "Multiplication result has correct cols");
    
    // Verify computations
    // c(0,0) = 1*7 + 2*9 + 3*11 = 7 + 18 + 33 = 58
    // c(0,1) = 1*8 + 2*10 + 3*12 = 8 + 20 + 36 = 64
    // c(1,0) = 4*7 + 5*9 + 6*11 = 28 + 45 + 66 = 139
    // c(1,1) = 4*8 + 5*10 + 6*12 = 32 + 50 + 72 = 154
    
    ASSERT(approxEqual(c(0, 0), 58.0), "Multiplication (0,0) = 58");
    ASSERT(approxEqual(c(0, 1), 64.0), "Multiplication (0,1) = 64");
    ASSERT(approxEqual(c(1, 0), 139.0), "Multiplication (1,0) = 139");
    ASSERT(approxEqual(c(1, 1), 154.0), "Multiplication (1,1) = 154");
    
    // Test 2: Dimension mismatch
    try {
        Matrix invalid = a * a;  // 2x3 * 2x3 invalid
        ASSERT(false, "Multiplication throws on incompatible dimensions");
    } catch (const std::invalid_argument&) {
        ASSERT(true, "Multiplication throws on incompatible dimensions");
    }
}

// ============================================================
// Test Suite 6: Operator==
// ============================================================
void testEquality() {
    std::cout << "\n========== Test Suite 6: Operator== (Equality) ==========\n";
    
    Matrix a(2, 2);
    a(0, 0) = 1.0; a(0, 1) = 2.0;
    a(1, 0) = 3.0; a(1, 1) = 4.0;
    
    Matrix b = a;
    Matrix c(2, 2);
    c(0, 0) = 1.0; c(0, 1) = 2.0;
    c(1, 0) = 3.0; c(1, 1) = 4.0;
    
    Matrix d(2, 2);
    d(0, 0) = 1.0; d(0, 1) = 2.0;
    d(1, 0) = 3.0; d(1, 1) = 5.0;  // Different element
    
    Matrix e(3, 3);
    
    ASSERT(a == b, "Equality: copies are equal");
    ASSERT(a == c, "Equality: different instances with same values");
    ASSERT(!(a == d), "Equality: different values returns false");
    ASSERT(!(a == e), "Equality: different dimensions returns false");
    
    // Test floating-point tolerance
    Matrix f(2, 2);
    f(0, 0) = 1.0 + 1e-10; f(0, 1) = 2.0;
    f(1, 0) = 3.0; f(1, 1) = 4.0;
    ASSERT(a == f, "Equality uses epsilon tolerance for floating-point");
}

// ============================================================
// Test Suite 7: Operator[]
// ============================================================
void testRowAccess() {
    std::cout << "\n========== Test Suite 7: Operator[] (Row Access) ==========\n";
    
    Matrix a(3, 4);
    a(0, 0) = 1.1; a(0, 1) = 1.2; a(0, 2) = 1.3; a(0, 3) = 1.4;
    a(1, 0) = 2.1; a(1, 1) = 2.2; a(1, 2) = 2.3; a(1, 3) = 2.4;
    a(2, 0) = 3.1; a(2, 1) = 3.2; a(2, 2) = 3.3; a(2, 3) = 3.4;
    
    // Test 1: Access row 0
    double* row0 = a[0];
    ASSERT(approxEqual(row0[0], 1.1), "Row access [0][0] returns correct value");
    ASSERT(approxEqual(row0[3], 1.4), "Row access [0][3] returns correct value");
    
    // Test 2: Access row 2
    double* row2 = a[2];
    ASSERT(approxEqual(row2[0], 3.1), "Row access [2][0] returns correct value");
    ASSERT(approxEqual(row2[3], 3.4), "Row access [2][3] returns correct value");
    
    // Test 3: Modify through row access
    row0[1] = 99.9;
    ASSERT(approxEqual(a(0, 1), 99.9), "Modifications through row access affect matrix");
    
    // Test 4: Bounds checking
    try {
        double* invalid = a[5];  // Out of bounds
        ASSERT(false, "Row access throws on out-of-bounds index");
    } catch (const std::out_of_range&) {
        ASSERT(true, "Row access throws on out-of-bounds index");
    }
}

// ============================================================
// Test Suite 8: at() with bounds checking
// ============================================================
void testAtMethod() {
    std::cout << "\n========== Test Suite 8: at() Method (Bounds Checking) ==========\n";
    
    Matrix a(2, 3);
    a(0, 0) = 1.1; a(0, 1) = 2.2; a(0, 2) = 3.3;
    a(1, 0) = 4.4; a(1, 1) = 5.5; a(1, 2) = 6.6;
    
    // Test 1: Valid access
    ASSERT(approxEqual(a.at(0, 0), 1.1), "at(0,0) returns correct value");
    ASSERT(approxEqual(a.at(1, 2), 6.6), "at(1,2) returns correct value");
    
    // Test 2: Bounds checking - row out of range
    try {
        a.at(5, 0);
        ASSERT(false, "at() throws on row out of bounds");
    } catch (const std::out_of_range&) {
        ASSERT(true, "at() throws on row out of bounds");
    }
    
    // Test 3: Bounds checking - col out of range
    try {
        a.at(0, 5);
        ASSERT(false, "at() throws on col out of bounds");
    } catch (const std::out_of_range&) {
        ASSERT(true, "at() throws on col out of bounds");
    }
}

// ============================================================
// Test Suite 9: Operator<< (Stream Insertion)
// ============================================================
void testStreamInsertion() {
    std::cout << "\n========== Test Suite 9: Operator<< (Stream Insertion) ==========\n";
    
    Matrix a(2, 2);
    a(0, 0) = 1.5; a(0, 1) = 2.5;
    a(1, 0) = 3.5; a(1, 1) = 4.5;
    
    std::stringstream ss;
    ss << a;
    std::string output = ss.str();
    
    ASSERT(output.find("2x2") != std::string::npos, "Stream output contains dimensions");
    ASSERT(output.find("1.50") != std::string::npos, "Stream output contains matrix value");
    ASSERT(output.length() > 0, "Stream output produces content");
}

// ============================================================
// Test Suite 10: Complex Operations
// ============================================================
void testComplexOperations() {
    std::cout << "\n========== Test Suite 10: Complex Operations ==========\n";
    
    Matrix a(2, 2);
    a.fill(1.0);
    
    Matrix b(2, 2);
    b.fill(2.0);
    
    // Test: (A + B) - A = B
    Matrix result = (a + b) - a;
    ASSERT(result == b, "Complex expression: (A + B) - A == B");
    
    // Test: 2*(A*I) where I is identity-like
    Matrix c(2, 2);
    c(0, 0) = 2.0; c(0, 1) = 0.0;
    c(1, 0) = 0.0; c(1, 1) = 2.0;
    
    Matrix scaled = a * c;
    ASSERT(approxEqual(scaled(0, 0), 2.0), "Complex: scalar-like multiplication works");
}

// ============================================================
// Test Suite 11: Move Semantics (V2)
// ============================================================
void testMoveSemantics() {
    std::cout << "\n========== Test Suite 11: Move Semantics (V2) ==========\n";
    
    // Test 1: Move constructor
    Matrix a(2, 2);
    a(0, 0) = 1.0; a(0, 1) = 2.0;
    a(1, 0) = 3.0; a(1, 1) = 4.0;
    
    Matrix b = std::move(a);  // Move constructor
    
    ASSERT(b.rows() == 2, "Move constructor transfers rows");
    ASSERT(b.cols() == 2, "Move constructor transfers cols");
    ASSERT(approxEqual(b(0, 0), 1.0), "Move constructor transfers data correctly");
    ASSERT(approxEqual(b(1, 1), 4.0), "Move constructor transfers all data");
    
    // Test 2: Source is in moved-from state
    ASSERT(a.rows() == 0, "Source after move has zero rows");
    ASSERT(a.cols() == 0, "Source after move has zero cols");
    
    // Test 3: Move assignment
    Matrix c(1, 1);
    c(0, 0) = 99.0;
    
    Matrix d(3, 3);
    d.fill(5.0);
    
    d = std::move(c);
    
    ASSERT(d.rows() == 1, "Move assignment changes dimensions (rows)");
    ASSERT(d.cols() == 1, "Move assignment changes dimensions (cols)");
    ASSERT(approxEqual(d(0, 0), 99.0), "Move assignment transfers data correctly");
    
    // Test 4: Source after move assignment is in moved-from state
    ASSERT(c.rows() == 0, "Source after move assignment has zero rows");
    ASSERT(c.cols() == 0, "Source after move assignment has zero cols");
    
    // Test 5: Move from temporary
    auto createMatrix = []() {
        Matrix temp(2, 3);
        temp(0, 0) = 1.1; temp(0, 1) = 2.2; temp(0, 2) = 3.3;
        temp(1, 0) = 4.4; temp(1, 1) = 5.5; temp(1, 2) = 6.6;
        return temp;  // Move constructor called
    };
    
    Matrix e = createMatrix();
    ASSERT(e.rows() == 2, "Move from temporary preserves dimensions");
    ASSERT(e.cols() == 3, "Move from temporary preserves dimensions");
    ASSERT(approxEqual(e(0, 0), 1.1), "Move from temporary preserves data");
    ASSERT(approxEqual(e(1, 2), 6.6), "Move from temporary preserves all data");
    
    // Test 6: Self-move assignment
    Matrix f(2, 2);
    f.fill(7.0);
    f = std::move(f);  // Should not crash
    ASSERT(approxEqual(f(0, 0), 7.0), "Self-move assignment is safe");
}

// ============================================================
// Main Test Runner
// ============================================================
int main() {
    std::cout << "\n#########################################\n";
    std::cout << "   MATRIX CLASS UNIT TEST SUITE\n";
    std::cout << "#########################################\n";
    
    Matrix::resetStats();
    
    // Run all test suites
    testConstructor();
    testCopyOperations();
    testAddition();
    testSubtraction();
    testMultiplication();
    testEquality();
    testRowAccess();
    testAtMethod();
    testStreamInsertion();
    testComplexOperations();
    testMoveSemantics();
    
    // Print summary
    std::cout << "\n#########################################\n";
    std::cout << "   TEST SUMMARY\n";
    std::cout << "#########################################\n";
    std::cout << "Total Tests:   " << test_count << "\n";
    std::cout << "Passed:        " << passed_count << " ✓\n";
    std::cout << "Failed:        " << failed_count << " ✗\n";
    
    if (failed_count == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED!\n";
        return 0;
    } else {
        std::cout << "\n❌ SOME TESTS FAILED\n";
        return 1;
    }
}
