#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>

/**
 * Simple Benchmarking Harness using std::chrono
 * 
 * Features:
 * - Easy-to-use timer class
 * - Multiple measurement runs
 * - Statistical analysis (min, max, mean, median)
 * - Formatted output
 * 
 * Usage:
 *   Benchmark timer;
 *   for (int i = 0; i < iterations; i++) {
 *       timer.start();
 *       // Code to benchmark
 *       timer.stop();
 *   }
 *   timer.report("Operation Name");
 */

class Benchmark {
private:
    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::duration<double, std::milli>;  // Milliseconds
    
    std::vector<Duration> measurements;
    Clock::time_point start_time;
    bool running;

public:
    /**
     * Constructor
     */
    Benchmark() : running(false) {}

    /**
     * Start the timer
     * Call once per iteration
     */
    void start() {
        start_time = Clock::now();
        running = true;
    }

    /**
     * Stop the timer and record the measurement
     * Call once per iteration
     */
    void stop() {
        if (!running) {
            std::cerr << "Warning: Timer not running!\n";
            return;
        }
        Clock::time_point end_time = Clock::now();
        Duration elapsed = end_time - start_time;
        measurements.push_back(elapsed);
        running = false;
    }

    /**
     * Get the number of measurements taken
     */
    int count() const {
        return measurements.size();
    }

    /**
     * Get minimum time (in milliseconds)
     */
    double min() const {
        if (measurements.empty()) return 0.0;
        return std::min_element(measurements.begin(), measurements.end())->count();
    }

    /**
     * Get maximum time (in milliseconds)
     */
    double max() const {
        if (measurements.empty()) return 0.0;
        return std::max_element(measurements.begin(), measurements.end())->count();
    }

    /**
     * Get average (mean) time (in milliseconds)
     */
    double mean() const {
        if (measurements.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& m : measurements) {
            sum += m.count();
        }
        return sum / measurements.size();
    }

    /**
     * Get median time (in milliseconds)
     */
    double median() const {
        if (measurements.empty()) return 0.0;
        
        std::vector<double> sorted;
        for (const auto& m : measurements) {
            sorted.push_back(m.count());
        }
        std::sort(sorted.begin(), sorted.end());
        
        if (sorted.size() % 2 == 0) {
            return (sorted[sorted.size() / 2 - 1] + sorted[sorted.size() / 2]) / 2.0;
        } else {
            return sorted[sorted.size() / 2];
        }
    }

    /**
     * Get standard deviation (in milliseconds)
     */
    double stddev() const {
        if (measurements.empty() || measurements.size() == 1) return 0.0;
        
        double mean_val = mean();
        double variance = 0.0;
        
        for (const auto& m : measurements) {
            double diff = m.count() - mean_val;
            variance += diff * diff;
        }
        variance /= (measurements.size() - 1);  // Sample variance
        
        return std::sqrt(variance);
    }

    /**
     * Get the total time for all measurements (in milliseconds)
     */
    double total() const {
        if (measurements.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& m : measurements) {
            sum += m.count();
        }
        return sum;
    }

    /**
 * Print a detailed report with reproducibility note
 */
void report(const std::string& name = "Benchmark") const {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << name << "\n";
    std::cout << std::string(60, '=') << "\n";
    
    if (measurements.empty()) {
        std::cout << "  No measurements taken\n";
        return;
    }
    
    std::cout << "  Iterations:       " << std::setw(10) << count() << "\n";
    std::cout << "  Total Time:       " << std::setw(10) << std::fixed << std::setprecision(6) 
              << total() << " ms\n";
    std::cout << "\n";
    std::cout << "  Min:              " << std::setw(10) << std::fixed << std::setprecision(6) 
              << min() << " ms\n";
    std::cout << "  Max:              " << std::setw(10) << std::fixed << std::setprecision(6) 
              << max() << " ms\n";
    std::cout << "  Mean:             " << std::setw(10) << std::fixed << std::setprecision(6) 
              << mean() << " ms\n";
    std::cout << "  Median:           " << std::setw(10) << std::fixed << std::setprecision(6) 
              << median() << " ms  (Recommended for reproducibility)\n";
                  << stddev() << " ms\n";
        std::cout << std::string(60, '=') << "\n\n";
    }

    /**
     * Print a simple one-line report
     */
    void report_simple(const std::string& name = "Benchmark") const {
        std::cout << name << ": "
                  << std::fixed << std::setprecision(6) << mean() << " ms "
                  << "(min: " << min() << ", max: " << max() << ")\n";
    }

    /**
     * Reset all measurements
     */
    void reset() {
        measurements.clear();
        running = false;
    }

    /**
     * Get raw measurement at index (in milliseconds)
     */
    double operator[](int index) const {
        if (index < 0 || index >= measurements.size()) {
            return 0.0;
        }
        return measurements[index].count();
    }

    /**
     * Clear only outliers, keeping results within [Q1-1.5*IQR, Q3+1.5*IQR]
     * Returns the number of outliers removed
     */
    int removeOutliers() {
        if (measurements.size() <= 4) return 0;  // Need at least 4 points
        
        std::vector<double> sorted;
        for (const auto& m : measurements) {
            sorted.push_back(m.count());
        }
        std::sort(sorted.begin(), sorted.end());
        
        size_t q1_idx = sorted.size() / 4;
        size_t q3_idx = (3 * sorted.size()) / 4;
        double Q1 = sorted[q1_idx];
        double Q3 = sorted[q3_idx];
        double IQR = Q3 - Q1;
        
        double lower = Q1 - 1.5 * IQR;
        double upper = Q3 + 1.5 * IQR;
        
        int removed = 0;
        for (auto it = measurements.begin(); it != measurements.end(); ) {
            if (it->count() < lower || it->count() > upper) {
                it = measurements.erase(it);
                ++removed;
            } else {
                ++it;
            }
        }
        return removed;
    }

    /**
     * Run a function multiple times and collect statistics
     * Useful for reproducible benchmarking with median reporting
     * 
     * Usage:
     *   Benchmark bench;
     *   bench.multiRun(100, [&]() {
     *       // Code to benchmark (timing is automatic)
     *       operation();
     *   });
     *   bench.report("Operation");
     */
    template <typename Func>
    void multiRun(int iterations, Func fn) {
        reset();
        for (int i = 0; i < iterations; ++i) {
            start();
            fn();
            stop();
        }
    }
};

/**
 * RAII-style timer for automatic timing of a scope
 * 
 * Usage:
 *   {
 *       ScopedTimer timer("MyOperation");
 *       // Code to benchmark
 *   }  // Automatically prints result
 */
class ScopedTimer {
private:
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start_time;
    std::string name;

public:
    ScopedTimer(const std::string& op_name) : name(op_name) {
        start_time = Clock::now();
    }

    ~ScopedTimer() {
        Clock::time_point end_time = Clock::now();
        auto elapsed = std::chrono::duration<double, std::milli>(end_time - start_time);
        std::cout << "[" << name << "] " << std::fixed << std::setprecision(6) 
                  << elapsed.count() << " ms\n";
    }
};

#endif // BENCHMARK_H
