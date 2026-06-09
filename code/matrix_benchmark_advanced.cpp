#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <cmath>
#include "matrix.h"

/**
 * Enhanced Google Benchmark Suite for Matrix Operations
 * 
 * Features:
 * - Chained operations (A + B + C + D)
 * - Various matrix sizes (10×10, 100×100, 1000×1000)
 * - Reproducible results (fixed seed)
 * - Statistical median-based analysis
 * - Real-world operation scenarios
 */

// ============================================================
// Reproducible Random Initialization
// ============================================================

class MatrixGenerator {
private:
    // Fixed seed for reproducibility
    static constexpr uint32_t SEED = 42;
    
public:
    /**
     * Generate a matrix with reproducible random values
     * Uses fixed seed for all calls
     */
    static Matrix generate(int rows, int cols, double min = 0.0, double max = 10.0) {
        std::mt19937 rng(SEED);
        std::uniform_real_distribution<double> dist(min, max);
        
        Matrix m(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                m(i, j) = dist(rng);
            }
        }
        return m;
    }
    
    /**
     * Generate multiple matrices with same seed
     * Ensures reproducibility across multiple calls
     */
    static std::vector<Matrix> generateMultiple(int count, int rows, int cols) {
        std::vector<Matrix> matrices;
        for (int i = 0; i < count; ++i) {
            matrices.push_back(generate(rows, cols));
        }
        return matrices;
    }
};

// ============================================================
// Benchmark: Chained Additions
// ============================================================

/**
 * Benchmark: A + B + C + D (4 operand chain)
 * Tests: Expression template optimization, move semantics
 */
static void BM_Chained_Addition_2x2(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(2, 2);
    Matrix B = MatrixGenerator::generate(2, 2);
    Matrix C = MatrixGenerator::generate(2, 2);
    Matrix D = MatrixGenerator::generate(2, 2);
    
    for (auto _ : state) {
        Matrix result = A + B + C + D;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Chained_Addition_10x10(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(10, 10);
    Matrix B = MatrixGenerator::generate(10, 10);
    Matrix C = MatrixGenerator::generate(10, 10);
    Matrix D = MatrixGenerator::generate(10, 10);
    
    for (auto _ : state) {
        Matrix result = A + B + C + D;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Chained_Addition_100x100(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(100, 100);
    Matrix B = MatrixGenerator::generate(100, 100);
    Matrix C = MatrixGenerator::generate(100, 100);
    Matrix D = MatrixGenerator::generate(100, 100);
    
    for (auto _ : state) {
        Matrix result = A + B + C + D;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Chained_Addition_500x500(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(500, 500);
    Matrix B = MatrixGenerator::generate(500, 500);
    Matrix C = MatrixGenerator::generate(500, 500);
    Matrix D = MatrixGenerator::generate(500, 500);
    
    for (auto _ : state) {
        Matrix result = A + B + C + D;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Chained_Addition_1000x1000(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(1000, 1000);
    Matrix B = MatrixGenerator::generate(1000, 1000);
    Matrix C = MatrixGenerator::generate(1000, 1000);
    Matrix D = MatrixGenerator::generate(1000, 1000);
    
    for (auto _ : state) {
        Matrix result = A + B + C + D;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Benchmark: Matrix Multiplication
// ============================================================

/**
 * Benchmark: Matrix Multiplication (C = A × B)
 * Tests: Cache behavior, complexity scaling
 * Note: For square matrices, complexity is O(n³)
 */

static void BM_Multiply_2x2(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(2, 2);
    Matrix B = MatrixGenerator::generate(2, 2);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Multiply_10x10(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(10, 10);
    Matrix B = MatrixGenerator::generate(10, 10);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Multiply_50x50(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(50, 50);
    Matrix B = MatrixGenerator::generate(50, 50);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Multiply_100x100(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(100, 100);
    Matrix B = MatrixGenerator::generate(100, 100);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Multiply_200x200(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(200, 200);
    Matrix B = MatrixGenerator::generate(200, 200);
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Benchmark: In-Place Operations
// ============================================================

/**
 * Benchmark: Chained In-Place Additions (A += B += C)
 * Tests: Efficiency of member operators vs free functions
 * Key: Should be faster than A = A + B + C due to no copies
 */

static void BM_InPlace_Chained_10x10(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(10, 10);
        Matrix B = MatrixGenerator::generate(10, 10);
        Matrix C = MatrixGenerator::generate(10, 10);
        state.ResumeTiming();
        
        A += B;
        A += C;
        benchmark::DoNotOptimize(A);
    }
}

static void BM_InPlace_Chained_100x100(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(100, 100);
        Matrix B = MatrixGenerator::generate(100, 100);
        Matrix C = MatrixGenerator::generate(100, 100);
        state.ResumeTiming();
        
        A += B;
        A += C;
        benchmark::DoNotOptimize(A);
    }
}

static void BM_InPlace_Chained_500x500(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(500, 500);
        Matrix B = MatrixGenerator::generate(500, 500);
        Matrix C = MatrixGenerator::generate(500, 500);
        state.ResumeTiming();
        
        A += B;
        A += C;
        benchmark::DoNotOptimize(A);
    }
}

// ============================================================
// Benchmark: Free Function vs In-Place (Scaling)
// ============================================================

/**
 * Compare free function (A + B) vs in-place (A += B)
 * across different matrix sizes
 */

static void BM_FreeFunc_vs_InPlace_10x10_Free(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(10, 10);
    Matrix B = MatrixGenerator::generate(10, 10);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_FreeFunc_vs_InPlace_10x10_InPlace(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(10, 10);
        Matrix B = MatrixGenerator::generate(10, 10);
        state.ResumeTiming();
        
        A += B;
        benchmark::DoNotOptimize(A);
    }
}

static void BM_FreeFunc_vs_InPlace_100x100_Free(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(100, 100);
    Matrix B = MatrixGenerator::generate(100, 100);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_FreeFunc_vs_InPlace_100x100_InPlace(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(100, 100);
        Matrix B = MatrixGenerator::generate(100, 100);
        state.ResumeTiming();
        
        A += B;
        benchmark::DoNotOptimize(A);
    }
}

static void BM_FreeFunc_vs_InPlace_500x500_Free(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(500, 500);
    Matrix B = MatrixGenerator::generate(500, 500);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_FreeFunc_vs_InPlace_500x500_InPlace(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix A = MatrixGenerator::generate(500, 500);
        Matrix B = MatrixGenerator::generate(500, 500);
        state.ResumeTiming();
        
        A += B;
        benchmark::DoNotOptimize(A);
    }
}

// ============================================================
// Benchmark: Real-World Scenario - Iterative Refinement
// ============================================================

/**
 * Simulates iterative refinement: X = X + ΔX (repeated)
 * Tests: Accumulation efficiency
 */

static void BM_Iterative_Refinement_100x100_100Iterations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix X = MatrixGenerator::generate(100, 100);
        std::vector<Matrix> deltas = MatrixGenerator::generateMultiple(100, 100, 100);
        state.ResumeTiming();
        
        for (const auto& delta : deltas) {
            X += delta;
        }
        benchmark::DoNotOptimize(X);
    }
}

static void BM_Iterative_Refinement_100x100_1000Iterations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Matrix X = MatrixGenerator::generate(100, 100);
        std::vector<Matrix> deltas = MatrixGenerator::generateMultiple(1000, 100, 100);
        state.ResumeTiming();
        
        for (const auto& delta : deltas) {
            X += delta;
        }
        benchmark::DoNotOptimize(X);
    }
}

// ============================================================
// Benchmark: Scaling Analysis (Fixed Operation)
// ============================================================

/**
 * Single addition operation across size range
 * Shows how time scales with matrix size
 */

static void BM_Addition_Scaling_2x2(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(2, 2);
    Matrix B = MatrixGenerator::generate(2, 2);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

static void BM_Addition_Scaling_10x10(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(10, 10);
    Matrix B = MatrixGenerator::generate(10, 10);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

static void BM_Addition_Scaling_50x50(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(50, 50);
    Matrix B = MatrixGenerator::generate(50, 50);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

static void BM_Addition_Scaling_100x100(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(100, 100);
    Matrix B = MatrixGenerator::generate(100, 100);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

static void BM_Addition_Scaling_500x500(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(500, 500);
    Matrix B = MatrixGenerator::generate(500, 500);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

static void BM_Addition_Scaling_1000x1000(benchmark::State& state) {
    Matrix A = MatrixGenerator::generate(1000, 1000);
    Matrix B = MatrixGenerator::generate(1000, 1000);
    for (auto _ : state) {
        Matrix r = A + B;
        benchmark::DoNotOptimize(r);
    }
}

// ============================================================
// Register Benchmarks
// ============================================================

// Chained Additions
BENCHMARK(BM_Chained_Addition_2x2);
BENCHMARK(BM_Chained_Addition_10x10);
BENCHMARK(BM_Chained_Addition_100x100);
BENCHMARK(BM_Chained_Addition_500x500);
BENCHMARK(BM_Chained_Addition_1000x1000);

// Matrix Multiplication
BENCHMARK(BM_Multiply_2x2);
BENCHMARK(BM_Multiply_10x10);
BENCHMARK(BM_Multiply_50x50);
BENCHMARK(BM_Multiply_100x100);
BENCHMARK(BM_Multiply_200x200);

// In-Place Operations
BENCHMARK(BM_InPlace_Chained_10x10);
BENCHMARK(BM_InPlace_Chained_100x100);
BENCHMARK(BM_InPlace_Chained_500x500);

// Free Function vs In-Place Comparison
BENCHMARK(BM_FreeFunc_vs_InPlace_10x10_Free);
BENCHMARK(BM_FreeFunc_vs_InPlace_10x10_InPlace);
BENCHMARK(BM_FreeFunc_vs_InPlace_100x100_Free);
BENCHMARK(BM_FreeFunc_vs_InPlace_100x100_InPlace);
BENCHMARK(BM_FreeFunc_vs_InPlace_500x500_Free);
BENCHMARK(BM_FreeFunc_vs_InPlace_500x500_InPlace);

// Iterative Refinement
BENCHMARK(BM_Iterative_Refinement_100x100_100Iterations);
BENCHMARK(BM_Iterative_Refinement_100x100_1000Iterations);

// Scaling Analysis
BENCHMARK(BM_Addition_Scaling_2x2);
BENCHMARK(BM_Addition_Scaling_10x10);
BENCHMARK(BM_Addition_Scaling_50x50);
BENCHMARK(BM_Addition_Scaling_100x100);
BENCHMARK(BM_Addition_Scaling_500x500);
BENCHMARK(BM_Addition_Scaling_1000x1000);

BENCHMARK_MAIN();
