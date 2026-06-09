#include "matrix.h"
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <iomanip>

/**
 * Google Benchmark Suite for Matrix Operations - WITH STATS LOGGING
 * 
 * Logs statistics about temporary objects (copies/moves) created during benchmarks.
 * Outputs to CSV file: benchmark_stats_OPTIMIZATION_LEVEL_TIMESTAMP.csv
 * 
 * Features:
 * - Tracks copy_count and move_count for each benchmark
 * - CSV export with timing and stats data
 * - Comparison of temporary object creation across operations
 * - Helps identify which operations create most temporaries
 */

// Global variables to capture stats
struct BenchmarkStats {
    std::string benchmark_name;
    int64_t total_copies = 0;
    int64_t total_moves = 0;
};

static std::vector<BenchmarkStats> stats_log;
static std::ofstream stats_file;

// ============================================================
// Benchmark 1: Matrix Addition (Simple 2-operand)
// ============================================================

static void BM_Addition_100x100(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
    
    // Record stats after benchmark
    BenchmarkStats stat;
    stat.benchmark_name = "Addition_100x100";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
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
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix result = A - B;
        benchmark::DoNotOptimize(result);
    }
    
    BenchmarkStats stat;
    stat.benchmark_name = "Subtraction_100x100";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
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
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix result = A * B;
        benchmark::DoNotOptimize(result);
    }
    
    BenchmarkStats stat;
    stat.benchmark_name = "Multiplication_50x50";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
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
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix temp = A;
        temp += B;
        benchmark::DoNotOptimize(temp);
    }
    
    BenchmarkStats stat;
    stat.benchmark_name = "InPlace_Addition_100x100";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
}

BENCHMARK(BM_InPlace_Addition_100x100);

// ============================================================
// Benchmark 5: Chained Operations (A + B + C)
// ============================================================

static void BM_Chained_Addition_3x(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    Matrix C(100, 100);
    A.fill(1.0);
    B.fill(2.0);
    C.fill(3.0);
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix result = A + B + C;
        benchmark::DoNotOptimize(result);
    }
    
    BenchmarkStats stat;
    stat.benchmark_name = "Chained_Addition_3x";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
}

BENCHMARK(BM_Chained_Addition_3x);

// ============================================================
// Benchmark 6: Mixed Operations (Add + Subtract + Multiply)
// ============================================================

static void BM_Mixed_Operations(benchmark::State& state) {
    Matrix A(50, 50);
    Matrix B(50, 50);
    Matrix C(50, 50);
    A.fill(1.5);
    B.fill(2.0);
    C.fill(0.5);
    
    Matrix::resetStats();
    
    for (auto _ : state) {
        Matrix temp1 = A + B;
        Matrix temp2 = temp1 - C;
        Matrix result = temp2 * A;
        benchmark::DoNotOptimize(result);
    }
    
    BenchmarkStats stat;
    stat.benchmark_name = "Mixed_Operations";
    stat.total_copies = Matrix::getCopyCount();
    stat.total_moves = Matrix::getMoveCount();
    
    stats_log.push_back(stat);
    std::cout << "\n  [STATS] " << stat.benchmark_name 
              << " | Copies: " << stat.total_copies 
              << " | Moves: " << stat.total_moves << "\n";
}

BENCHMARK(BM_Mixed_Operations);

// ============================================================
// Custom Reporter for Stats
// ============================================================

class StatsReporter : public benchmark::BenchmarkReporter {
public:
    bool ReportContext(const Context& context) override {
        return true;
    }

    void ReportRuns(const std::vector<Run>& reports) override {
        // Called after each benchmark run
    }

    void Finalize() override {
        // Export stats to CSV
        exportStatsToCSV();
    }

private:
    void exportStatsToCSV() {
        // Generate filename with timestamp
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", &tstruct);
        
        std::string filename = "results/benchmark_stats_" + std::string(timestamp) + ".csv";
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << " for writing\n";
            return;
        }
        
        // CSV Header
        file << "Benchmark_Name,Total_Copies,Total_Moves,Copy_to_Move_Ratio,Efficiency\n";
        
        for (const auto& stat : stats_log) {
            double ratio = stat.total_moves > 0 ? 
                (double)stat.total_copies / stat.total_moves : stat.total_copies;
            double efficiency = (stat.total_copies + stat.total_moves) > 0 ?
                (double)stat.total_moves / (stat.total_copies + stat.total_moves) : 0.0;
            
            file << stat.benchmark_name << ","
                 << stat.total_copies << ","
                 << stat.total_moves << ","
                 << std::fixed << std::setprecision(2) << ratio << ","
                 << std::fixed << std::setprecision(2) << efficiency << "\n";
        }
        
        file.close();
        
        std::cout << "\n[OK] Statistics exported to: " << filename << "\n";
        printStatsSummary();
    }
    
    void printStatsSummary() {
        std::cout << "\n========== Temporary Object Statistics Summary ==========\n";
        std::cout << std::left << std::setw(30) << "Benchmark"
                  << std::right << std::setw(12) << "Copies"
                  << std::setw(12) << "Moves"
                  << std::setw(12) << "Ratio"
                  << std::setw(12) << "Efficiency\n";
        std::cout << std::string(78, '-') << "\n";
        
        for (const auto& stat : stats_log) {
            double ratio = stat.total_moves > 0 ? 
                (double)stat.total_copies / stat.total_moves : stat.total_copies;
            double efficiency = (stat.total_copies + stat.total_moves) > 0 ?
                (double)stat.total_moves / (stat.total_copies + stat.total_moves) : 0.0;
            
            std::cout << std::left << std::setw(30) << stat.benchmark_name
                      << std::right << std::setw(12) << stat.total_copies
                      << std::setw(12) << stat.total_moves
                      << std::setw(12) << std::fixed << std::setprecision(2) << ratio
                      << std::setw(12) << std::fixed << std::setprecision(2) << efficiency << "\n";
        }
        std::cout << std::string(78, '=') << "\n";
    }
};

// ============================================================
// Custom Main to Register Stats Reporter
// ============================================================

int main(int argc, char** argv) {
    // Run benchmarks with default reporter
    benchmark::Initialize(&argc, argv);
    
    // Register custom stats reporter
    StatsReporter reporter;
    benchmark::RunSpecifiedBenchmarks(&reporter);
    
    return 0;
}
