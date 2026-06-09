# Optimization Level Benchmarking - Setup Complete ✓

## What You've Created

### 1. **PowerShell Script** (Main Runner)
- **File:** `benchmark_optimization_levels.ps1`
- **Purpose:** Compile and run benchmarks at -O0, -O2, and -O3 optimization levels
- **Usage:** `.\benchmark_optimization_levels.ps1 -Speed normal`
- **Speed Options:** 
  - `fast` (50ms/benchmark, ~10 min total)
  - `normal` (500ms/benchmark, ~25 min total) ← **Recommended**
  - `thorough` (2s/benchmark, ~100 min total)

### 2. **Bash Script** (Cross-Platform)
- **File:** `benchmark_optimization_levels.sh`
- **Purpose:** Same functionality for Linux/macOS
- **Usage:** `./benchmark_optimization_levels.sh normal`

### 3. **Python Analysis Tool**
- **File:** `analyze_optimization_levels.py`
- **Purpose:** Parse and compare CSV results across optimization levels
- **Usage:** `python analyze_optimization_levels.py results/benchmark_*_O*.csv`
- **Output:**
  - Individual statistics per optimization level
  - Pairwise speedup comparisons (O2 vs O0, O3 vs O0, O3 vs O2)
  - Benchmark-by-benchmark breakdown
  - Performance recommendations

### 4. **Documentation**
- **File:** `OPTIMIZATION_LEVELS_README.md`
- **Content:** Complete guide with examples, troubleshooting, and next steps

## Current Execution Status

**Status:** ✅ **RUNNING NOW** (started 2026-06-02 15:50:59)

The benchmark suite is currently executing:
1. ✅ Compilation with -O0: **COMPLETE**
2. ⏳ Benchmark execution with -O0: **IN PROGRESS**
3. ⏳ Compilation & execution with -O2: **PENDING**
4. ⏳ Compilation & execution with -O3: **PENDING**

**Estimated completion time:** ~25 minutes total (Speed: normal)

## Generated Files During Execution

### Compiled Executables (in `bin/`)
```
bin\matrix_google_benchmark_O0.exe      (15 benchmarks, -O0)
bin\matrix_google_benchmark_O2.exe      (15 benchmarks, -O2)  
bin\matrix_google_benchmark_O3.exe      (15 benchmarks, -O3)
bin\matrix_benchmark_advanced_O0.exe    (33 benchmarks, -O0)
bin\matrix_benchmark_advanced_O2.exe    (33 benchmarks, -O2)
bin\matrix_benchmark_advanced_O3.exe    (33 benchmarks, -O3)
```

### Object Files (in `obj/`)
```
obj\matrix_O0.obj, obj\matrix_O2.obj, obj\matrix_O3.obj
obj\matrix_google_benchmark_O0.obj, ...O2.obj, ...O3.obj
obj\matrix_benchmark_advanced_O0.obj, ...O2.obj, ...O3.obj
```

### Result CSVs (in `results/`)
```
results\benchmark_original_O0_<timestamp>.csv
results\benchmark_original_O2_<timestamp>.csv
results\benchmark_original_O3_<timestamp>.csv
results\benchmark_advanced_O0_<timestamp>.csv
results\benchmark_advanced_O2_<timestamp>.csv
results\benchmark_advanced_O3_<timestamp>.csv
```

## What Gets Measured

### Per Optimization Level:
- **48 Total Benchmarks** (15 original + 33 advanced)
- **Categories:**
  1. **Original Benchmarks (15):** Basic operations, constructors, chained operations, scaling
  2. **Advanced Benchmarks (33):** Chained additions, matrix multiplication, in-place ops, free vs member, iterative refinement, scaling analysis

### Metrics Collected:
- Benchmark name
- CPU time (primary metric)
- Wall clock time
- Iterations
- Aggregate statistics

## Performance Comparison Overview

### Expected Results Pattern

| Optimization | Relative Speed | Use Case |
|---|---|---|
| -O0 | 100% (baseline) | Debug builds (no optimization) |
| -O2 | ~130-150% faster | Standard production |
| -O3 | ~140-160% faster | Maximum performance |

**Typical Findings:**
- -O0 to -O2: **~30-50% speedup** (most important gains)
- -O2 to -O3: **~5-15% speedup** (diminishing returns)
- -O0 to -O3: **~50-100% speedup** (combined benefit)

## Next Steps (After Execution Completes)

### 1. **Verify Results Generated**
```powershell
Get-ChildItem results\benchmark_*_O*.csv
```

### 2. **Analyze Results**
```bash
python analyze_optimization_levels.py results/benchmark_*_O*.csv
```

### 3. **View Detailed Analysis**
The script will output:
- Statistics for each optimization level
- Speedup calculations
- Performance recommendations
- Benchmark-level breakdown

### 4. **Export to Excel** (Optional)
```powershell
Invoke-Item results\benchmark_original_O*.csv
```
Create charts:
- Time vs Optimization Level (O0, O2, O3)
- Benchmark categories color-coded
- Scaling visualization

### 5. **Compare with Previous Runs**
Save results with descriptive names:
```bash
mv results/benchmark_*_O*.csv results/baseline_optimization_comparison/
```

## Key Files Reference

| File | Purpose | Status |
|------|---------|--------|
| benchmark_optimization_levels.ps1 | Main Windows script | ✅ Ready |
| benchmark_optimization_levels.sh | Main Linux/macOS script | ✅ Ready |
| analyze_optimization_levels.py | Results analyzer | ✅ Ready |
| OPTIMIZATION_LEVELS_README.md | Complete documentation | ✅ Ready |

## Troubleshooting

### If Benchmark Hangs
- Press Ctrl+C to stop current benchmark
- Check `bin/matrix_google_benchmark_O<X>.exe --help`
- Verify benchmark binary works independently

### If No CSV Generated
- Check if executables created in `bin/` directory
- Verify results directory has write permissions
- Run single executable manually to test

### If Results Seem Wrong
- Check system clock for time jumps
- Verify CPU not throttling (no power saving)
- Use `thorough` speed setting for more iterations

## Integration with Existing Benchmarking

These scripts **complement** existing benchmarking:
- Original `run_all_benchmarks.ps1` → Runs with current build config
- New `benchmark_optimization_levels.ps1` → Compares across optimization levels
- Both can be run independently
- Results can be combined for comprehensive analysis

## Technical Details

### Compilation Flags Used

**-O0 (No Optimization)**
- Fast compilation, useful for debugging
- Large binary size
- Slowest execution

**-O2 (Balanced Optimization)**
- Industry standard for production builds
- Good compilation speed
- ~30-50% faster than -O0

**-O3 (Maximum Optimization)**
- Aggressive optimizations
- Slower compilation
- ~5-15% faster than -O2
- May increase code size

### Library Paths Fixed
- **Issue:** Benchmark libraries not found
- **Solution:** Use absolute paths to libbenchmark.a and libbenchmark_main.a in `C:\benchmark\build\src\`
- **Status:** ✅ Fixed in both PowerShell and Bash scripts

---

**Created:** 2026-06-02  
**Purpose:** Compare C++ matrix benchmark performance across -O0, -O2, and -O3 optimization levels  
**Expected Duration:** 25 minutes (normal mode)  
**Output:** CSV files + analysis report
