#include "matrix.h"
#include "benchmark.h"
#include <iostream>
#include <iomanip>

/**
 * Matrix Operations Benchmarking Suite
 * 
 * Demonstrates:
 * 1. Simple std::chrono benchmarking
 * 2. Statistical analysis (min, max, mean, median)
 * 3. Comparing V2 (Move Semantics) performance
 * 4. Comparing V3 (Compound Operators) performance
 */

int main() {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "  Matrix Operations Benchmarking Suite\n";
    std::cout << "  Using std::chrono for high-resolution timing\n";
    std::cout << std::string(80, '=') << "\n";

    // ============================================================
    // Benchmark 1: Addition Operation
    // ============================================================
    std::cout << "\n--- Benchmark 1: Matrix Addition ---\n";
    std::cout << "Creating 100×100 matrices and performing 1000 additions\n\n";

    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);

    Benchmark add_benchmark;
    const int ADD_ITERATIONS = 1000;

    for (int i = 0; i < ADD_ITERATIONS; ++i) {
        add_benchmark.start();
        Matrix result = A + B;  // Uses V3 free function + V2 move semantics
        add_benchmark.stop();
    }

    add_benchmark.report("Matrix Addition (A + B)");
    std::cout << "  Per operation average: " << std::fixed << std::setprecision(9)
              << (add_benchmark.mean() * 1000) << " microseconds\n";

    // ============================================================
    // Benchmark 2: Subtraction Operation
    // ============================================================
    std::cout << "\n--- Benchmark 2: Matrix Subtraction ---\n";
    std::cout << "Creating 100×100 matrices and performing 1000 subtractions\n\n";

    Benchmark sub_benchmark;
    const int SUB_ITERATIONS = 1000;

    for (int i = 0; i < SUB_ITERATIONS; ++i) {
        sub_benchmark.start();
        Matrix result = A - B;  // Uses V3 free function + V2 move semantics
        sub_benchmark.stop();
    }

    sub_benchmark.report("Matrix Subtraction (A - B)");

    // ============================================================
    // Benchmark 3: Multiplication Operation
    // ============================================================
    std::cout << "\n--- Benchmark 3: Matrix Multiplication ---\n";
    std::cout << "Creating 50×50 matrices and performing 100 multiplications\n";
    std::cout << "(Note: O(n³) complexity, so fewer iterations)\n\n";

    Matrix C(50, 50);
    Matrix D(50, 50);
    C.fill(1.5);
    D.fill(2.0);

    Benchmark mul_benchmark;
    const int MUL_ITERATIONS = 100;

    for (int i = 0; i < MUL_ITERATIONS; ++i) {
        mul_benchmark.start();
        Matrix result = C * D;  // Matrix multiplication
        mul_benchmark.stop();
    }

    mul_benchmark.report("Matrix Multiplication (C * D)");

    // ============================================================
    // Benchmark 4: In-place Addition (V3: +=)
    // ============================================================
    std::cout << "\n--- Benchmark 4: In-place Addition (V3) ---\n";
    std::cout << "Testing += operator performance vs + operator\n";
    std::cout << "Creating fresh copy for each iteration to ensure fair comparison\n\n";

    Benchmark inplace_add_benchmark;
    const int INPLACE_ITERATIONS = 1000;

    for (int i = 0; i < INPLACE_ITERATIONS; ++i) {
        Matrix temp = A;  // Fresh copy for each iteration
        inplace_add_benchmark.start();
        temp += B;  // In-place addition
        inplace_add_benchmark.stop();
    }

    inplace_add_benchmark.report("In-place Addition (A += B)");

    // ============================================================
    // Benchmark 5: Comparison - Free Function vs In-place
    // ============================================================
    std::cout << "\n--- Benchmark 5: Comparison - Free Function (+) vs In-place (+=) ---\n";
    std::cout << "Both create new result, but different implementation approaches\n\n";

    std::cout << "Free Function (+):     " << std::fixed << std::setprecision(6) 
              << add_benchmark.mean() << " ms average\n";
    std::cout << "In-place (+=):         " << std::fixed << std::setprecision(6) 
              << inplace_add_benchmark.mean() << " ms average\n";

    double difference = add_benchmark.mean() - inplace_add_benchmark.mean();
    double percent = (difference / add_benchmark.mean()) * 100.0;

    if (difference > 0) {
        std::cout << "In-place is faster by " << std::fixed << std::setprecision(2) 
                  << percent << "% (" << difference << " ms)\n";
    } else {
        std::cout << "Free function is faster by " << std::fixed << std::setprecision(2) 
                  << (-percent) << "% (" << (-difference) << " ms)\n";
    }

    // ============================================================
    // Benchmark 6: Chained Operations (V2 Move Semantics Benefit)
    // ============================================================
    std::cout << "\n--- Benchmark 6: Chained Operations (V2 Benefits) ---\n";
    std::cout << "Testing: Result = (A + B) - (A - B)  [Complex expression]\n";
    std::cout << "Move semantics optimizes intermediate temporaries\n\n";

    Benchmark chain_benchmark;
    const int CHAIN_ITERATIONS = 500;

    for (int i = 0; i < CHAIN_ITERATIONS; ++i) {
        chain_benchmark.start();
        Matrix result = (A + B) - (A - B);  // V2 optimizes temporaries here
        chain_benchmark.stop();
    }

    chain_benchmark.report("Chained Operations");

    // ============================================================
    // Benchmark 7: Loop Accumulation (V3 += Benefits)
    // ============================================================
    std::cout << "\n--- Benchmark 7: Loop Accumulation (V3 += Benefits) ---\n";
    std::cout << "Accumulating 100 100×100 matrices\n";
    std::cout << "Testing += vs repeated + operations\n\n";

    // Create test matrices
    std::vector<Matrix> matrices;
    for (int i = 0; i < 100; ++i) {
        Matrix m(100, 100);
        m.fill(1.0);
        matrices.push_back(m);
    }

    // Benchmark: Using += in loop
    Benchmark accumulate_benchmark;
    const int ACCUMULATE_RUNS = 10;

    for (int run = 0; run < ACCUMULATE_RUNS; ++run) {
        Matrix sum(100, 100);
        sum.fill(0.0);

        accumulate_benchmark.start();
        for (const auto& m : matrices) {
            sum += m;  // In-place accumulation
        }
        accumulate_benchmark.stop();
    }

    accumulate_benchmark.report("Loop Accumulation (sum += matrices[i])");

    // ============================================================
    // Benchmark 8: Copy Constructor Performance
    // ============================================================
    std::cout << "\n--- Benchmark 8: Copy Constructor ---\n";
    std::cout << "Deep copying 100×100 matrices\n";
    std::cout << "This is expensive - used in intermediate operations without move\n\n";

    Benchmark copy_benchmark;
    const int COPY_ITERATIONS = 1000;

    for (int i = 0; i < COPY_ITERATIONS; ++i) {
        copy_benchmark.start();
        Matrix copy = A;  // Copy constructor
        copy_benchmark.stop();
    }

    copy_benchmark.report("Copy Constructor");

    // ============================================================
    // Statistical Summary
    // ============================================================
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "  STATISTICAL SUMMARY\n";
    std::cout << std::string(80, '=') << "\n\n";

    std::cout << "Operation Summary Table:\n";
    std::cout << std::left << std::setw(30) << "Operation"
              << std::setw(15) << "Mean (ms)"
              << std::setw(15) << "Min (ms)"
              << std::setw(15) << "Max (ms)\n";
    std::cout << std::string(75, '-') << "\n";

    std::cout << std::left << std::setw(30) << "Addition (+)"
              << std::setw(15) << std::fixed << std::setprecision(6) << add_benchmark.mean()
              << std::setw(15) << add_benchmark.min()
              << std::setw(15) << add_benchmark.max() << "\n";

    std::cout << std::left << std::setw(30) << "Subtraction (-)"
              << std::setw(15) << std::fixed << std::setprecision(6) << sub_benchmark.mean()
              << std::setw(15) << sub_benchmark.min()
              << std::setw(15) << sub_benchmark.max() << "\n";

    std::cout << std::left << std::setw(30) << "Multiplication (*)"
              << std::setw(15) << std::fixed << std::setprecision(6) << mul_benchmark.mean()
              << std::setw(15) << mul_benchmark.min()
              << std::setw(15) << mul_benchmark.max() << "\n";

    std::cout << std::left << std::setw(30) << "In-place Add (+=)"
              << std::setw(15) << std::fixed << std::setprecision(6) << inplace_add_benchmark.mean()
              << std::setw(15) << inplace_add_benchmark.min()
              << std::setw(15) << inplace_add_benchmark.max() << "\n";

    std::cout << std::left << std::setw(30) << "Chained Operations"
              << std::setw(15) << std::fixed << std::setprecision(6) << chain_benchmark.mean()
              << std::setw(15) << chain_benchmark.min()
              << std::setw(15) << chain_benchmark.max() << "\n";

    std::cout << std::left << std::setw(30) << "Copy Constructor"
              << std::setw(15) << std::fixed << std::setprecision(6) << copy_benchmark.mean()
              << std::setw(15) << copy_benchmark.min()
              << std::setw(15) << copy_benchmark.max() << "\n";

    // ============================================================
    // Performance Insights
    // ============================================================
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "  PERFORMANCE INSIGHTS\n";
    std::cout << std::string(80, '=') << "\n\n";

    std::cout << "1. Addition vs Subtraction:\n";
    double add_mean = add_benchmark.mean();
    double sub_mean = sub_benchmark.mean();
    std::cout << "   Both should be similar (same algorithmic complexity)\n";
    std::cout << "   Difference: " << std::fixed << std::setprecision(3) 
              << std::abs(add_mean - sub_mean) << " ms\n\n";

    std::cout << "2. Free Function (+) Implementation (V3):\n";
    std::cout << "   • Creates copy of left operand\n";
    std::cout << "   • Uses += member operator\n";
    std::cout << "   • Returns result (move semantics optimizes this)\n";
    std::cout << "   • No code duplication - logic in +=\n\n";

    std::cout << "3. Chained Operations (V2 Benefit):\n";
    double chain_mean = chain_benchmark.mean();
    double approx_sequential = add_benchmark.mean() * 2;  // Two operations
    std::cout << "   Chained time: " << std::fixed << std::setprecision(6) << chain_mean << " ms\n";
    std::cout << "   Sequential (2×Add): ~" << approx_sequential << " ms\n";
    std::cout << "   Move semantics avoids copying temporaries\n\n";

    std::cout << "4. Standard Deviation Analysis:\n";
    std::cout << "   Addition StdDev:  " << std::fixed << std::setprecision(6) 
              << add_benchmark.stddev() << " ms\n";
    std::cout << "   (Consistency of timing across iterations)\n\n";

    std::cout << "5. Copy Constructor Cost:\n";
    std::cout << "   Each copy of 100×100 matrix: " << std::fixed << std::setprecision(6) 
              << copy_benchmark.mean() << " ms\n";
    std::cout << "   (This is why V2 move semantics is important)\n\n";

    std::cout << std::string(80, '=') << "\n\n";

    return 0;
}
