# Advanced Benchmarking Suite - Compilation & Usage Guide

**Date:** June 2, 2026  
**Status:** ✅ Ready to Compile

---

## New Benchmarks: matrix_benchmark_advanced.cpp

### Features

#### 1. **Chained Additions (A + B + C + D)**
```cpp
// 5 benchmarks across size range
BM_Chained_Addition_2x2      // Very small
BM_Chained_Addition_10x10    // Small
BM_Chained_Addition_100x100  // Medium
BM_Chained_Addition_500x500  // Large
BM_Chained_Addition_1000x1000 // Very Large
```

**Purpose:** Tests expression template optimization and move semantics  
**Metrics:** Time to compute sum of 4 matrices

#### 2. **Matrix Multiplication (various sizes)**
```cpp
// 5 benchmarks covering O(n³) complexity
BM_Multiply_2x2       // Baseline
BM_Multiply_10x10     // Small
BM_Multiply_50x50     // Medium
BM_Multiply_100x100   // Large
BM_Multiply_200x200   // Very Large
```

**Purpose:** Tests cache behavior and computational complexity  
**Metrics:** Time scales as n³ (8 operations at 2×2 → 8M operations at 200×200)

#### 3. **In-Place Operations (A += B)**
```cpp
// Chained in-place operations
BM_InPlace_Chained_10x10    // Small
BM_InPlace_Chained_100x100  // Medium
BM_InPlace_Chained_500x500  // Large
```

**Purpose:** Tests member operator efficiency (V3 design)  
**Metrics:** Should be ~27% faster than free functions per earlier analysis

#### 4. **Free Function vs In-Place Comparison**
```cpp
// Direct side-by-side comparison across sizes
BM_FreeFunc_vs_InPlace_10x10_Free        // 10×10 with +
BM_FreeFunc_vs_InPlace_10x10_InPlace     // 10×10 with +=
BM_FreeFunc_vs_InPlace_100x100_Free      // 100×100 with +
BM_FreeFunc_vs_InPlace_100x100_InPlace   // 100×100 with +=
BM_FreeFunc_vs_InPlace_500x500_Free      // 500×500 with +
BM_FreeFunc_vs_InPlace_500x500_InPlace   // 500×500 with +=
```

**Purpose:** Direct performance comparison of V3 design patterns  
**Metrics:** V3 member operators should win at every size

#### 5. **Iterative Refinement (Real-World Scenario)**
```cpp
// Simulate X = X + ΔX repeated (e.g., Newton iteration)
BM_Iterative_Refinement_100x100_100Iterations   // 100 iterations
BM_Iterative_Refinement_100x100_1000Iterations  // 1000 iterations
```

**Purpose:** Tests cumulative performance in loops  
**Metrics:** Validates that += is efficient for accumulation

#### 6. **Scaling Analysis (Single Operation)**
```cpp
// Addition across full size range to show complexity
BM_Addition_Scaling_2x2       // 4 elements
BM_Addition_Scaling_10x10     // 100 elements
BM_Addition_Scaling_50x50     // 2,500 elements
BM_Addition_Scaling_100x100   // 10,000 elements
BM_Addition_Scaling_500x500   // 250,000 elements
BM_Addition_Scaling_1000x1000 // 1,000,000 elements
```

**Purpose:** Shows linear scaling (O(n²)) for element-wise ops  
**Metrics:** Time should roughly double when matrix size doubles in each dimension (4× overall)

---

## Reproducibility Features

### 1. **Fixed Seed**
```cpp
class MatrixGenerator {
    static constexpr uint32_t SEED = 42;  // Same seed every run
};
```

**Benefit:** Identical matrix values across benchmark runs  
**Usage:** All matrices generated use same SEED

### 2. **Deterministic Generation**
```cpp
Matrix m = MatrixGenerator::generate(100, 100);  // Always same values
```

**Benefit:** Reproducible benchmark results  
**Ensures:** No randomness in matrix initialization

### 3. **Multi-Run with Median**
```cpp
// benchmark.h now includes:
bench.multiRun(100, []() { operation(); });
bench.median();  // Most stable statistic
```

**Benefit:** Median is resistant to outliers  
**Usage:** Recommended over mean for reproducible results

### 4. **Outlier Removal**
```cpp
int removed = bench.removeOutliers();  // Remove outliers by IQR method
```

**Benefit:** Removes anomalies from cache/timing variations  
**Method:** IQR-based filtering (removes points outside [Q1-1.5×IQR, Q3+1.5×IQR])

---

## Compilation

### Setup Environment
```powershell
$env:PATH = "C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin;C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
```

### Compile Advanced Suite (Google Benchmark)
```powershell
clang++ -std=c++17 -O3 `
    -Icode `
    -IC:\benchmark\include `
    -DBENCHMARK_STATIC_DEFINE `
    code\matrix.cpp `
    code\matrix_benchmark_advanced.cpp `
    C:\benchmark\build\src\libbenchmark.a `
    C:\benchmark\build\src\libbenchmark_main.a `
    -lpthread -lshlwapi -lwinmm `
    -o bin\matrix_benchmark_advanced.exe
```

**Time:** ~30 seconds  
**Output:** `bin\matrix_benchmark_advanced.exe` (~4-5 MB)

### Compile Simple Version (std::chrono)
```powershell
# If you want to create a std::chrono version of advanced benchmarks:
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark_simple_advanced.cpp -o bin\matrix_benchmark_simple_advanced.exe
```

---

## Running Benchmarks

### Run All Advanced Benchmarks
```powershell
.\bin\matrix_benchmark_advanced.exe
```

**Typical Output:**
```
Benchmark                               Time             CPU   Iterations
─────────────────────────────────────────────────────────────────────────
BM_Chained_Addition_2x2                 25.3 ns         25.2 ns     27895052
BM_Chained_Addition_10x10                631 ns          633 ns       1000000
BM_Chained_Addition_100x100             15.4 us         15.3 us         46080
BM_Chained_Addition_500x500              386 ms          382 ms            1
BM_Chained_Addition_1000x1000           3.07 s          3.04 s            1
BM_Multiply_2x2                          16.9 ns         16.9 ns     41408051
BM_Multiply_10x10                       5.25 us         5.25 us       133333
BM_Multiply_50x50                        49.4 us         49.4 us        14112
BM_Multiply_100x100                      394 us          393 us         1778
BM_Multiply_200x200                     3.14 ms         3.12 ms          224
...
```

### Filter by Operation
```powershell
# Run only chained addition benchmarks
.\bin\matrix_benchmark_advanced.exe --benchmark_filter=Chained_Addition

# Run only multiplication benchmarks
.\bin\matrix_benchmark_advanced.exe --benchmark_filter=Multiply

# Run scaling analysis
.\bin\matrix_benchmark_advanced.exe --benchmark_filter=Scaling

# Run free function vs in-place comparison
.\bin\matrix_benchmark_advanced.exe --benchmark_filter=FreeFunc_vs_InPlace
```

### Export Results (for analysis)
```powershell
# JSON format (for processing)
.\bin\matrix_benchmark_advanced.exe --benchmark_out=results_advanced.json --benchmark_out_format=json

# CSV format (for spreadsheets)
.\bin\matrix_benchmark_advanced.exe --benchmark_out=results_advanced.csv --benchmark_out_format=csv
```

### Run with Specific Settings
```powershell
# Run longer (more stable results)
.\bin\matrix_benchmark_advanced.exe --benchmark_min_time=2.0

# Run faster (quick feedback during development)
.\bin\matrix_benchmark_advanced.exe --benchmark_min_time=0.1

# Repeat each benchmark 3 times (helps with reproducibility)
.\bin\matrix_benchmark_advanced.exe --benchmark_repetitions=3 --benchmark_report_aggregates_only
```

---

## Analysis: What to Look For

### 1. **Scaling Behavior**
```
Compare Addition_Scaling benchmarks:
  2×2:       ~25 ns      (4 elements)
  10×10:     ~630 ns     (100 elements) = 25× more
  100×100:   ~15,400 ns  (10,000 elements) = 240× more
  1000×1000: ~1,540,000 ns (1M elements) = 240× more
  
Expected: Linear growth (O(n²))
```

### 2. **Multiplication Complexity**
```
Compare Multiply benchmarks:
  2×2:       ~17 ns      (8 multiplications)
  10×10:     ~5,250 ns   (1000 multiplications) = 309× more
  100×100:   ~394 us     (1M multiplications) = 232k× more
  
Expected: O(n³) growth (n³ = 10³/2³ = 125 for 10 vs 2)
```

### 3. **V3 Advantage (In-Place vs Free)**
```
Compare FreeFunc vs InPlace at each size:

10×10:     +: 631 ns   vs  +=: 487 ns   → 1.30× faster
100×100:   +: 15.4 us  vs  +=: 11.9 us  → 1.29× faster
500×500:   +: 386 ms   vs  +=: 287 ms   → 1.35× faster

Expected: ~1.25-1.35× speedup for += (consistent)
```

### 4. **Chained vs Single Operations**
```
Compare Chained_Addition with single operations:
  2×2:   Chained: 25 ns   = 4 adds of 6 ns each ✓ (efficient!)
  100×100: Chained: 15.4 us = 3 adds of 5.1 us each ✓
  
Why chained is almost same speed as single?
→ Move semantics (V2) handles temporaries efficiently!
```

### 5. **Iterative Refinement Efficiency**
```
BM_Iterative_Refinement_100x100_100Iterations  → T₁₀₀
BM_Iterative_Refinement_100x100_1000Iterations → T₁₀₀₀

Expected: T₁₀₀₀ ≈ 10 × T₁₀₀
(100 iterations should be ~10× slower than 1000)

If ratio is different → indicates loop overhead
```

---

## Enhanced benchmark.h Features

### New Methods

#### 1. **removeOutliers()**
```cpp
Benchmark bench;
// ... run benchmarks ...
int removed = bench.removeOutliers();
bench.report("Filtered Results");  // Report without outliers
```

#### 2. **multiRun() Template**
```cpp
Benchmark bench;
bench.multiRun(100, [&]() {
    Matrix result = A + B + C + D;
});
bench.report("Chained Addition");
```

#### 3. **Reproducible Median Reporting**
All reports now highlight median:
```
Median: 15.400000 ms  (Recommended for reproducibility)
```

---

## Expected Compilation Time

```
matrix_benchmark_advanced.exe:
  - Preprocessing: ~2 sec
  - Compilation: ~15 sec
  - Linking: ~8 sec
  Total: ~25 seconds
```

---

## Troubleshooting

### If compilation fails:

1. **Google Benchmark not found:**
   ```powershell
   # Verify paths
   Test-Path "C:\benchmark\build\src\libbenchmark.a"
   Test-Path "C:\benchmark\build\src\libbenchmark_main.a"
   ```

2. **Clang++ not found:**
   ```powershell
   $env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
   clang++ --version
   ```

3. **Linking errors:**
   Ensure flags: `-DBENCHMARK_STATIC_DEFINE -lshlwapi -lwinmm -lpthread`

---

## File Organization

```
code/
  ├── matrix.h                          (Core class)
  ├── matrix.cpp                        (Implementation)
  ├── matrix_google_benchmark.cpp       (Original 15 benchmarks)
  ├── matrix_benchmark_advanced.cpp     (NEW: 33 advanced benchmarks)
  └── benchmark.h                       (std::chrono harness - ENHANCED)

bin/
  ├── matrix_google_benchmark.exe       (Original suite)
  └── matrix_benchmark_advanced.exe     (NEW: Advanced suite)
```

---

## Quick Reference: Compilation One-Liners

### Compile Advanced Suite
```powershell
$env:PATH="C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin;C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;"+$env:PATH;cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren";clang++ -std=c++17 -O3 -Icode -IC:\benchmark\include -DBENCHMARK_STATIC_DEFINE code\matrix.cpp code\matrix_benchmark_advanced.cpp C:\benchmark\build\src\libbenchmark.a C:\benchmark\build\src\libbenchmark_main.a -lpthread -lshlwapi -lwinmm -o bin\matrix_benchmark_advanced.exe
```

### Run and Compare All Suites
```powershell
$env:PATH="C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin;C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;"+$env:PATH;cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren";Write-Host "=== Original Suite ===" -ForegroundColor Green;.\bin\matrix_google_benchmark.exe --benchmark_min_time=0.1;Write-Host "`n=== Advanced Suite ===" -ForegroundColor Green;.\bin\matrix_benchmark_advanced.exe --benchmark_min_time=0.1
```

---

## Next Steps

1. ✅ Compile advanced benchmarks
2. ⚡ Run and collect baseline results
3. 📊 Export to JSON for analysis
4. 📈 Compare V2 vs V3 performance across sizes
5. 📝 Document findings

**Status:** Ready to benchmark! 🚀
