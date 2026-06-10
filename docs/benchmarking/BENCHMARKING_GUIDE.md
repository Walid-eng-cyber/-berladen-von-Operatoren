# Benchmarking Harness Guide

This directory includes a complete benchmarking setup for measuring Matrix operation performance.

## Quick Start

### Simple Version (std::chrono)

**File:** `benchmark.h`  
**Features:**
- No external dependencies (std::chrono is part of C++ standard library)
- Simple, easy to understand
- Perfect for quick performance analysis
- Provides min, max, mean, median, standard deviation

**Usage:**
```cpp
#include "benchmark.h"

Benchmark timer;

for (int i = 0; i < 1000; ++i) {
    timer.start();
    // Code to benchmark
    Matrix result = A + B;
    timer.stop();
}

timer.report("Matrix Addition");
```

**Output:**
```
============================================================
  Matrix Addition
============================================================
  Iterations:              1000
  Total Time:         123.456789 ms

  Min:                  0.100000 ms
  Max:                  0.200000 ms
  Mean:                 0.123457 ms
  Median:               0.123000 ms
  StdDev:               0.010000 ms
============================================================
```

### Benchmark Classes

#### 1. `Benchmark` Class

**Methods:**
- `start()` - Begin timing
- `stop()` - End timing, record measurement
- `count()` - Number of iterations
- `min()` - Minimum time
- `max()` - Maximum time
- `mean()` - Average time
- `median()` - Middle value
- `stddev()` - Standard deviation
- `total()` - Sum of all measurements
- `report(name)` - Detailed report
- `report_simple(name)` - One-line report
- `reset()` - Clear measurements
- `operator[](index)` - Access raw measurement

**Example:**
```cpp
Benchmark b;

for (int i = 0; i < 1000; ++i) {
    b.start();
    // ... code ...
    b.stop();
}

b.report("Operation");
std::cout << "Mean: " << b.mean() << " ms\n";
std::cout << "Iterations: " << b.count() << "\n";
```

#### 2. `ScopedTimer` Class

**Automatic timing using RAII pattern**

**Example:**
```cpp
{
    ScopedTimer timer("Matrix Addition");
    // Code to benchmark
    Matrix result = A + B;
}  // Automatically prints timing

// Output: [Matrix Addition] 0.123456 ms
```

**Benefits:**
- No manual start/stop needed
- Automatic cleanup
- Great for quick prototyping

---

## Running the Benchmark Suite

### Compile:
```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
clang++ -std=c++17 -O2 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe
```

**Note:** Use `-O2` or `-O3` optimization for accurate real-world performance measurements.

### Run:
```powershell
.\bin\matrix_benchmark.exe
```

---

## Benchmark Suite Contents

### `matrix_benchmark.cpp` - Complete Suite

**Benchmarks included:**

1. **Addition Operation (100×100, 1000 iterations)**
   - Tests `operator+` (free function using V3 and V2)
   - Measures temporary creation and move

2. **Subtraction Operation (100×100, 1000 iterations)**
   - Compares with addition performance
   - Same complexity class

3. **Multiplication Operation (50×50, 100 iterations)**
   - O(n³) complexity, so fewer iterations
   - Baseline for more expensive operations

4. **In-place Addition (100×100, 1000 iterations)**
   - Tests `operator+=` (V3 member operator)
   - Compares with free function version

5. **Comparison - Free Function vs In-place**
   - Analyzes performance difference
   - Shows if += is measurably faster

6. **Chained Operations**
   - Tests: `Result = (A + B) - (A - B)`
   - Demonstrates V2 move semantics benefit
   - Shows temporary optimization

7. **Loop Accumulation (100 matrices)**
   - Real-world use case: `sum += matrices[i]`
   - Shows V3 += operator efficiency

8. **Copy Constructor**
   - Baseline: cost of deep copying
   - Explains why move semantics matters

---

## Performance Metrics Explained

### Min / Max
- **Min:** Best-case timing (CPU cache hot, minimal interference)
- **Max:** Worst-case timing (cache miss, OS interference)
- **Note:** Max can be 2-10× min due to system variability

### Mean (Average)
- Best overall estimate of typical performance
- Robust to outliers with enough samples

### Median
- Middle value when sorted
- Better than mean for skewed distributions
- Less affected by outliers

### Standard Deviation (StdDev)
- Measures variability across runs
- **Low StdDev:** Consistent performance
- **High StdDev:** Inconsistent (system interference)
- **Good sign:** StdDev << Mean (performance stable)

---

## Interpreting Results

### Example Output Analysis:

```
Mean: 0.123456 ms
Min:  0.100000 ms
Max:  0.200000 ms
StdDev: 0.010000 ms
```

**Interpretation:**
- Typical operation takes ~0.123 ms
- Performance range: 0.1 - 0.2 ms
- Variability is small (0.01 << 0.123), so stable
- ~2× variation (max/min) is normal due to OS scheduling

### V2 (Move Semantics) Benefit:

If chained operations are close to 2× addition time:
- ✅ V2 is working well (temporaries not copied)

If chained operations are much less than 2× addition time:
- ⚠️ Compiler optimizations may have eliminated operations

### V3 (Compound Operators) Benefit:

If `+=` is significantly faster than `+`:
- ✅ V3 saves the copy overhead

If performance is similar:
- ✅ Compiler is optimizing both equally (still good)
- Code cleanliness (DRY) is the main V3 benefit

---

## Advanced: Using Optimizations

### Compile with Optimization:

```powershell
# Development (debug symbols, no optimization):
clang++ -std=c++17 -g -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark_debug.exe

# Optimized (release build):
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe

# With additional optimizations:
clang++ -std=c++17 -O3 -march=native -flto -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark_ultra.exe
```

**Notes:**
- `-O0`: No optimization (slow, for debugging)
- `-O1`: Basic optimization
- `-O2`: Recommended balance
- `-O3`: Aggressive optimization (may take longer to compile)
- `-march=native`: Use CPU-specific optimizations
- `-flto`: Link-time optimization

---

## Optional: Google Benchmark Integration

**For advanced benchmarking with more features:**

### Install Google Benchmark:

```powershell
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ..
cmake --build "build" --config Release
```

### Create Google Benchmark Suite:

```cpp
#include <benchmark/benchmark.h>
#include "matrix.h"

static void BM_Addition(benchmark::State& state) {
    Matrix A(100, 100);
    Matrix B(100, 100);
    A.fill(2.5);
    B.fill(3.7);
    
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition);

BENCHMARK_MAIN();
```

### Compile with Google Benchmark:

```powershell
clang++ -std=c++17 -O3 -Icode matrix.cpp benchmark_suite.cpp -Ibenchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o benchmark_suite.exe
```

---

## Benchmarking Best Practices

### 1. Multiple Runs
- Do many iterations (at least 100)
- Average results reduce noise
- Statistical analysis matters

### 2. Warm Up
- First few runs may have cache misses
- Skip warmup iterations or do warm-up loop first

### 3. Prevent Optimization
- Use results so compiler doesn't eliminate them
- Use `benchmark::DoNotOptimize()` (Google Benchmark)
- Print results (std::chrono version)

### 4. Control Environment
- Close other programs
- Disable power saving features
- Run multiple times, compare results

### 5. Measure What Matters
- Focus on bottlenecks
- Compare before/after changes
- Use realistic data sizes

### 6. Document Results
- Record hardware (CPU, RAM)
- Record compiler and flags
- Record date and time
- Keep results for trend analysis

---

## Example: Creating Custom Benchmark

```cpp
#include "benchmark.h"
#include "matrix.h"

// Benchmark matrix operations of various sizes
void benchmark_by_size() {
    int sizes[] = {10, 50, 100, 500, 1000};
    
    for (int size : sizes) {
        Matrix A(size, size);
        Matrix B(size, size);
        A.fill(1.5);
        B.fill(2.5);
        
        Benchmark timer;
        for (int i = 0; i < 100; ++i) {
            timer.start();
            Matrix result = A + B;
            timer.stop();
        }
        
        std::cout << "Size " << size << "x" << size << ": "
                  << timer.mean() << " ms\n";
    }
}
```

---

## Troubleshooting

### Timing is Inconsistent (High StdDev)
- Close other programs
- Increase number of iterations
- Use `-O3` optimization

### All Operations Very Fast (< 0.001 ms)
- Compiler may be eliminating code
- Print results to prevent optimization
- Check assembly: `clang++ -S`

### Multiplication Much Slower than Addition
- ✓ Expected! Multiplication is O(n³), addition is O(n²)
- Try smaller matrices for comparable times

---

## Quick Reference

### Compile Benchmark:
```bash
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe
```

### Run Benchmark:
```bash
.\bin\matrix_benchmark.exe
```

### Use in Your Code:
```cpp
#include "benchmark.h"

Benchmark timer;
for (int i = 0; i < 1000; ++i) {
    timer.start();
    // Your code here
    timer.stop();
}
timer.report("My Operation");
```

---

**Happy Benchmarking!** 🚀
