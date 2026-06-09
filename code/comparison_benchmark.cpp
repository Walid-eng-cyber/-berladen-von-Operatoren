#include "matrix_v1.h"
#include "matrix_v2.h"
#include "matrix_v3.h"
#include <iostream>
#include <chrono>
#include <iomanip>

/**
 * Comparison Benchmark: V1 vs V2 vs V3
 * 
 * Tests all three implementations with the same operations
 * to demonstrate performance improvements from move semantics
 * and compound operators.
 */

void separator() {
    std::cout << "\n" << std::string(70, '=') << "\n\n";
}

// ============================================================================
// V1 TESTS
// ============================================================================

void test_v1_simple_addition() {
    std::cout << "V1: Simple Addition (100x100)\n";
    MatrixV1::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV1 a(100, 100);
    MatrixV1 b(100, 100);
    a.fill(2.5);
    b.fill(1.5);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV1 result = a + b;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV1::printStats();
}

void test_v1_accumulation() {
    std::cout << "V1: Accumulation Loop (100x100, 50 iterations)\n";
    MatrixV1::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV1 result(100, 100);
    result.fill(0.0);
    
    MatrixV1 temp(100, 100);
    temp.fill(0.5);
    
    for (int i = 0; i < 50; ++i) {
        result = result + temp;  // Uses free function: copy + temp creation
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV1::printStats();
}

void test_v1_chained_addition() {
    std::cout << "V1: Chained Addition (100x100): result = a + b + c\n";
    MatrixV1::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV1 a(100, 100);
    MatrixV1 b(100, 100);
    MatrixV1 c(100, 100);
    a.fill(1.0);
    b.fill(2.0);
    c.fill(3.0);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV1 result = a + b + c;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV1::printStats();
}

// ============================================================================
// V2 TESTS
// ============================================================================

void test_v2_simple_addition() {
    std::cout << "V2: Simple Addition (100x100)\n";
    MatrixV2::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV2 a(100, 100);
    MatrixV2 b(100, 100);
    a.fill(2.5);
    b.fill(1.5);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV2 result = a + b;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV2::printStats();
}

void test_v2_accumulation() {
    std::cout << "V2: Accumulation Loop (100x100, 50 iterations)\n";
    MatrixV2::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV2 result(100, 100);
    result.fill(0.0);
    
    MatrixV2 temp(100, 100);
    temp.fill(0.5);
    
    for (int i = 0; i < 50; ++i) {
        result = result + temp;  // Uses free function: copy + move optimization
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV2::printStats();
}

void test_v2_chained_addition() {
    std::cout << "V2: Chained Addition (100x100): result = a + b + c\n";
    MatrixV2::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV2 a(100, 100);
    MatrixV2 b(100, 100);
    MatrixV2 c(100, 100);
    a.fill(1.0);
    b.fill(2.0);
    c.fill(3.0);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV2 result = a + b + c;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV2::printStats();
}

// ============================================================================
// V3 TESTS
// ============================================================================

void test_v3_simple_addition() {
    std::cout << "V3: Simple Addition (100x100)\n";
    MatrixV3::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV3 a(100, 100);
    MatrixV3 b(100, 100);
    a.fill(2.5);
    b.fill(1.5);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV3 result = a + b;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV3::printStats();
}

void test_v3_accumulation() {
    std::cout << "V3: Accumulation Loop (100x100, 50 iterations) - USING +=\n";
    MatrixV3::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV3 result(100, 100);
    result.fill(0.0);
    
    MatrixV3 temp(100, 100);
    temp.fill(0.5);
    
    for (int i = 0; i < 50; ++i) {
        result += temp;  // Uses compound operator: no temporaries!
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV3::printStats();
}

void test_v3_chained_addition() {
    std::cout << "V3: Chained Addition (100x100): result = a + b + c\n";
    MatrixV3::resetStats();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    MatrixV3 a(100, 100);
    MatrixV3 b(100, 100);
    MatrixV3 c(100, 100);
    a.fill(1.0);
    b.fill(2.0);
    c.fill(3.0);
    
    for (int i = 0; i < 100; ++i) {
        MatrixV3 result = a + b + c;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Time: " << duration_us.count() << " µs\n";
    MatrixV3::printStats();
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << "MATRIX CLASS COMPARISON: V1 vs V2 vs V3\n";
    std::cout << std::string(70, '=') << "\n";
    
    // V1 Tests
    std::cout << "\n--- V1: Basic Copy Semantics ---\n";
    test_v1_simple_addition();
    separator();
    test_v1_accumulation();
    separator();
    test_v1_chained_addition();
    
    // V2 Tests
    std::cout << "\n--- V2: Copy + Move Semantics ---\n";
    test_v2_simple_addition();
    separator();
    test_v2_accumulation();
    separator();
    test_v2_chained_addition();
    
    // V3 Tests
    std::cout << "\n--- V3: Move Semantics + Compound Operators ---\n";
    test_v3_simple_addition();
    separator();
    test_v3_accumulation();
    separator();
    test_v3_chained_addition();
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "Benchmark Complete!\n";
    std::cout << "Key Observations:\n";
    std::cout << "  - V1: Accumulation uses copy semantics (inefficient)\n";
    std::cout << "  - V2: Move semantics reduce copies (faster)\n";
    std::cout << "  - V3: Compound operators (+=) eliminate temporaries (fastest)\n";
    std::cout << std::string(70, '=') << "\n\n";
    
    return 0;
}
