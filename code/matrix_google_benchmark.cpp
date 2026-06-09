#include "matrix.h"
#include <benchmark/benchmark.h>

/**
 * Google Benchmark Suite for Matrix Operations
 * 
 * Features:
 * - Automatic iteration count based on time (default 1s)
 * - Better reporting and comparison
 * - Command-line options for filtering and customization
 * - Automatic garbage collection between runs
 * 
 * Usage:
 *   matrix_google_benchmark.exe              # Run all benchmarks
 *   matrix_google_benchmark.exe --help       # Show options
 *   matrix_google_benchmark.exe --benchmark_filter=Add  # Run specific benchmark
 */

// ============================================================
// Benchmark 1: Matrix Addition
// ============================================================

static void BM_Addition_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        // Prevent compiler from optimizing away the computation
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition_100x100);

// ============================================================
// Benchmark 2: Matrix Subtraction
// ============================================================

static void BM_Subtraction_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A - B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Subtraction_100x100);

// ============================================================
// Benchmark 3: Matrix Multiplication
// ============================================================

static void BM_Multiplication_50x50(benchmark::State& state) {
    Matrix A(50, 50);
    Matrix B(50, 50);
    A.fill(1.5);
    B.fill(2.0);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Multiplication_50x50);

// ============================================================
// Benchmark 4: In-place Addition (+=)
// ============================================================

static void BM_InPlace_Addition_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        // Create a fresh copy for each iteration
        Matrix temp = A;
        temp += B;
        benchmark::DoNotOptimize(temp);
    }
}

BENCHMARK(BM_InPlace_Addition_100x100);

// ============================================================
// Benchmark 5: Chained Operations (V2 Benefit)
// ============================================================

static void BM_Chained_Operations(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = (A + B) - (A - B);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Chained_Operations);

// ============================================================
// Benchmark 6: Copy Constructor
// ============================================================

static void BM_Copy_Constructor_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    A.fill(2.5);
    
    for (auto _ : state) {
        Matrix copy = A;
        benchmark::DoNotOptimize(copy);
    }
}

BENCHMARK(BM_Copy_Constructor_100x100);

// ============================================================
// Benchmark 7: Move Constructor (V2 Benefit)
// ============================================================

static void BM_Move_Constructor_100x100(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A(100, 100);
        A.fill(2.5);
        state.ResumeTiming();
        
        Matrix moved = std::move(A);
        benchmark::DoNotOptimize(moved);
    }
}

BENCHMARK(BM_Move_Constructor_100x100);

// ============================================================
// Benchmark 8: Various Matrix Sizes (Addition)
// ============================================================

static void BM_Addition_10x10(benchmark::State& state) {
    Matrix A(10, 10);
    Matrix B(10, 10);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition_10x10);

static void BM_Addition_50x50(benchmark::State& state) {
    Matrix A(50, 50);
    Matrix B(50, 50);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition_50x50);

static void BM_Addition_500x500(benchmark::State& state) {
    Matrix A(500, 500);
    Matrix B(500, 500);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition_500x500);

// ============================================================
// Benchmark 9: Loop Accumulation (V3 Benefit)
// ============================================================

static void BM_Loop_Accumulation(benchmark::State& state) {
    std::vector<Matrix> matrices;
    for (int i = 0; i < 100; ++i) {
        Matrix m(100, 100);
        m.fill(1.0);
        matrices.push_back(m);
    }
    
    for (auto _ : state) {
        Matrix sum(100, 100);
        sum.fill(0.0);
        
        for (const auto& m : matrices) {
            sum += m;
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_Loop_Accumulation);

// ============================================================
// Benchmark 10: Copy Assignment
// ============================================================

static void BM_Copy_Assignment_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        A = B;
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(BM_Copy_Assignment_100x100);

// ============================================================
// Benchmark 11: Move Assignment
// ============================================================

static void BM_Move_Assignment_100x100(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A(100, 100);
        Matrix B(100, 100);
        A.fill(2.5);
        B.fill(3.7);
        state.ResumeTiming();
        
        A = std::move(B);
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(BM_Move_Assignment_100x100);

// ============================================================
// Benchmark 12: Comparison: + vs +=
// ============================================================

static void BM_Free_Function_Plus(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Free_Function_Plus)->Unit(benchmark::kMicrosecond);

static void BM_Member_Operator_PlusEqual(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        state.PauseTiming();
        Matrix temp = A;
        state.ResumeTiming();
        
        temp += B;
        benchmark::DoNotOptimize(temp);
    }
}

BENCHMARK(BM_Member_Operator_PlusEqual)->Unit(benchmark::kMicrosecond);

// ============================================================
// Entry Point
// ============================================================

BENCHMARK_MAIN();
