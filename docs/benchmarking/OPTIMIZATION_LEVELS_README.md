# Optimization Level Benchmarking Guide

Compare benchmark performance across different compiler optimization levels: **-O0** (no optimization), **-O2** (balanced optimization), and **-O3** (maximum optimization).

## Quick Start

### Windows (PowerShell)
```powershell
# Fast (50ms per benchmark, ~10 min total)
.\benchmark_optimization_levels.ps1 -Speed fast

# Normal (500ms per benchmark, ~25 min total) - RECOMMENDED
.\benchmark_optimization_levels.ps1 -Speed normal

# Thorough (2s per benchmark, ~100 min total)
.\benchmark_optimization_levels.ps1 -Speed thorough

# Skip compilation if already compiled
.\benchmark_optimization_levels.ps1 -Speed normal -SkipCompile
```

### Linux/macOS (Bash)
```bash
# Make script executable
chmod +x benchmark_optimization_levels.sh

# Run with different speeds
./benchmark_optimization_levels.sh fast       # ~10 minutes
./benchmark_optimization_levels.sh normal     # ~25 minutes (RECOMMENDED)
./benchmark_optimization_levels.sh thorough   # ~100 minutes
```

### Cross-platform (Python)
Coming soon - alternative Python implementation for maximum portability

## What It Does

### 1. Compilation Phase
For each optimization level (**-O0**, **-O2**, **-O3**):
- Compiles `matrix.cpp` with the specified optimization flag
- Compiles `matrix_google_benchmark.cpp` (15 benchmarks)
- Compiles `matrix_benchmark_advanced.cpp` (33 benchmarks)
- Creates separate executables for each level

**Generated executables:**
```
bin/
├── matrix_google_benchmark_O0.exe      (15 benchmarks, no optimization)
├── matrix_google_benchmark_O2.exe      (15 benchmarks, -O2)
├── matrix_google_benchmark_O3.exe      (15 benchmarks, -O3)
├── matrix_benchmark_advanced_O0.exe    (33 benchmarks, no optimization)
├── matrix_benchmark_advanced_O2.exe    (33 benchmarks, -O2)
└── matrix_benchmark_advanced_O3.exe    (33 benchmarks, -O3)
```

### 2. Execution Phase
For each executable:
- Runs benchmarks with specified minimum time (50ms/500ms/2s)
- Exports results as CSV files
- Saves each optimization level's results separately

**Generated CSV files:**
```
results/
├── benchmark_original_O0_20260602_145312.csv
├── benchmark_original_O2_20260602_145912.csv
├── benchmark_original_O3_20260602_150512.csv
├── benchmark_advanced_O0_20260602_145400.csv
├── benchmark_advanced_O2_20260602_150000.csv
└── benchmark_advanced_O3_20260602_150600.csv
```

## Analyze Results

### Python Analysis Tool (Recommended)
```bash
# Analyze all optimization levels
python analyze_optimization_levels.py results/benchmark_*_O*.csv

# Analyze specific files
python analyze_optimization_levels.py results/benchmark_original_O*.csv
python analyze_optimization_levels.py results/benchmark_advanced_O*.csv
```

**Output includes:**
- Individual statistics for each optimization level (min, max, mean, median, stdev)
- Pairwise comparisons (O2 vs O0, O3 vs O0, O3 vs O2)
- Speedup calculations
- Benchmark-by-benchmark performance breakdown
- Recommendations

### Example Output
```
╔════════════════════════════════════════════════════════════╗
║   Optimization Level Performance Analysis                  ║
╚════════════════════════════════════════════════════════════╝

Optimization levels found: O0, O2, O3

O0:
  Benchmarks: 15
  Min:        156.2 ns
  Max:        6.89 ms
  Mean:       42.53 µs
  Median:     18.46 µs
  Stdev:      98.24 µs

O2:
  Benchmarks: 15
  Min:        124.1 ns
  Max:        4.32 ms
  Mean:       28.19 µs
  Median:     12.37 µs
  Stdev:      63.41 µs

O3:
  Benchmarks: 15
  Min:        118.6 ns
  Max:        3.87 ms
  Mean:       25.84 µs
  Median:     11.12 µs
  Stdev:      57.18 µs

O2 vs O0:
  O2 mean: 28.19 µs
  ✓ 1.51× faster (+50.8%)

O3 vs O0:
  O3 mean: 25.84 µs
  ✓ 1.65× faster (+64.7%)

O3 vs O2:
  O3 mean: 25.84 µs
  ✓ 1.09× faster (+8.4%)
```

## Understanding the Results

### Optimization Levels

| Flag | Name | Use Case | Compile Time | Runtime Performance |
|------|------|----------|--------------|---------------------|
| `-O0` | No Optimization | Debugging | Fastest | Slowest |
| `-O2` | Balanced Optimization | Production | Normal | Good (90% of O3) |
| `-O3` | Maximum Optimization | High Performance | Slower | Best (~10% faster than O2) |

### Performance Insights

1. **O2 vs O0**: Usually **30-50% faster** - significant speedup for production
2. **O3 vs O2**: Usually **5-15% faster** - diminishing returns, may increase code size
3. **O3 vs O0**: Usually **50-100% faster** - combined benefit of all optimizations

### When to Use Each

- **-O0**: Development, debugging, CI/CD when speed doesn't matter
- **-O2**: Standard production (good balance of speed and compile time)
- **-O3**: Performance-critical code (if compile time is not a concern)

## Advanced Usage

### Recompile Specific Optimization
```powershell
# Manually recompile just O3
$OptFlag = "-O3"
$OptLabel = "O3"
# ... (see benchmark_optimization_levels.ps1 for full compilation commands)
```

### Combine with Other Analysis
```bash
# Export both original and advanced results
python analyze_optimization_levels.py \
  results/benchmark_original_O*.csv \
  results/benchmark_advanced_O*.csv

# Compare specific benchmark types
python analyze_optimization_levels.py results/benchmark_*_O2_*.csv results/benchmark_*_O3_*.csv
```

### Direct CSV Inspection
```powershell
# View raw CSV data
Get-Content results/benchmark_original_O2_*.csv | Select-Object -First 20

# Count benchmarks per level
(Get-Content results/benchmark_*_O0_*.csv | Measure-Object -Line).Lines
```

## Troubleshooting

### Compilation Fails
- Ensure LLVM/Clang is installed and in PATH
- Verify Google Benchmark headers in `C:\benchmark\include`
- Check object files in `obj/` directory are writable

### CSV Files Empty
- Verify benchmark executables run: `.\bin\matrix_google_benchmark_O2.exe --help`
- Check Google Benchmark library links correctly
- Try with `-SkipCompile` flag after manual successful compilation

### No Performance Difference Between Levels
- May indicate benchmarks are too small or I/O bound
- Try with `thorough` speed setting for longer benchmark times
- Check CPU is not throttling (power saving mode)

## Performance Tuning Tips

1. **Eliminate Outliers**: Use median instead of mean
2. **Multiple Runs**: Use `thorough` setting for ~10 runs per benchmark
3. **Fixed Seed**: All benchmarks use SEED=42 for reproducibility
4. **Warm-up**: Benchmarks warm up before measuring
5. **Compare Consistently**: Always compare same benchmark types (original vs advanced)

## Files Created/Modified

### New Scripts
- `benchmark_optimization_levels.ps1` - Main PowerShell script
- `benchmark_optimization_levels.sh` - Main Bash script
- `analyze_optimization_levels.py` - Analysis tool

### Generated Files (per run)
- `bin/matrix_*_O{0,2,3}.exe` - Compiled executables
- `obj/matrix_*_O{0,2,3}.obj` - Object files
- `results/benchmark_*_O{0,2,3}_*.csv` - Result CSVs

### Persistent Directories
- `bin/` - All compiled executables
- `obj/` - Intermediate object files
- `results/` - All benchmark result CSVs

## Example Workflow

```powershell
# 1. Run benchmarks at all optimization levels (normal speed, ~25 min)
.\benchmark_optimization_levels.ps1 -Speed normal

# 2. Analyze results
python analyze_optimization_levels.py results/benchmark_*_O*.csv

# 3. Save analysis to file
python analyze_optimization_levels.py results/benchmark_*_O*.csv | Tee-Object -FilePath results/optimization_analysis.txt

# 4. Open results in Excel for visualization
Invoke-Item results\benchmark_original_O*.csv
```

## Next Steps

- **Export to Excel**: Open CSV files to create charts (Time vs Optimization Level)
- **Compare Matrices Sizes**: Filter benchmarks by size (2×2, 100×100, 500×500)
- **Track Over Time**: Run again after code optimizations to measure impact
- **Document Findings**: Save analysis report for team reference

---

**Questions?** Review the main benchmark documentation:
- [BENCHMARK_SCRIPTS_README.md](documentation/BENCHMARK_SCRIPTS_README.md)
- [ADVANCED_BENCHMARKING_GUIDE.md](documentation/ADVANCED_BENCHMARKING_GUIDE.md)
- [BENCHMARK_SETUP_INSTRUMENTATION_SUMMARY.md](documentation/BENCHMARK_SETUP_INSTRUMENTATION_SUMMARY.md)
