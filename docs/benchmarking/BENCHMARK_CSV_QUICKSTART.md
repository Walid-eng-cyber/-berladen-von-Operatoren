# Benchmark CSV Scripts - Quick Start Guide

**3 Scripts. 1 Goal: Turn Benchmarks into Insights!**

---

## What You Have

| Script | Purpose | Platform |
|--------|---------|----------|
| `run_all_benchmarks.ps1` | Run benchmarks & export CSV | Windows (PowerShell) |
| `run_benchmarks.py` | Run benchmarks & export CSV | Any (Python 3.6+) |
| `run_benchmarks.sh` | Run benchmarks & export CSV | Linux/macOS (Bash) |
| `analyze_results.py` | Analyze & visualize results | Any (Python) |

---

## 30-Second Quick Start

### Windows
```powershell
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
.\run_all_benchmarks.ps1 -Speed normal
```

### Linux/macOS
```bash
cd "path/to/project"
chmod +x run_benchmarks.sh
./run_benchmarks.sh normal
```

### Any Platform (Python)
```bash
python run_benchmarks.py
```

**Result:** `results/benchmark_combined_*.csv` (ready for analysis)

---

## Step-by-Step Workflow

### Step 1: Run Benchmarks (5-25 min)
```powershell
# Windows PowerShell
.\run_all_benchmarks.ps1 -Speed normal

# Linux/macOS Bash
./run_benchmarks.sh normal

# Python (any platform)
python run_benchmarks.py
```

**Output:**
```
✓ Exported: results/benchmark_original_2026-06-02_HHMMSS.csv
✓ Exported: results/benchmark_advanced_2026-06-02_HHMMSS.csv
✓ Combined results: results/benchmark_combined_2026-06-02_HHMMSS.csv
✓ Analysis report: results/analysis_2026-06-02_HHMMSS.txt
```

### Step 2: View Analysis Report (1 min)
```powershell
# PowerShell
cat "results\analysis_*.txt"

# Bash
cat results/analysis_*.txt

# Windows Explorer
Start-Process "results\analysis_*.txt"
```

**Shows:** Summary, category breakdown, V2/V3 comparisons

### Step 3: Analyze Results (2-5 min)
```bash
# Detailed analysis with statistics
python analyze_results.py results/benchmark_combined_*.csv

# Or analyze all results in a directory
python analyze_results.py --dir results/
```

**Output:** Detailed report with:
- Category statistics
- V2 move semantics speedup
- V3 compound operator speedup
- Scaling verification (O(n²) check)

### Step 4: Import to Excel (5 min)
```powershell
# Open CSV in Excel
Start-Process "results\benchmark_combined_*.csv"

# Or manually: File → Open → benchmark_combined_*.csv
```

**Create charts:**
- Time vs Matrix Size (X: size, Y: time) → should be linear
- Free vs In-Place (compare columns) → V3 should be faster
- Copy vs Move (compare columns) → V2 should be faster

---

## Common Tasks

### Task 1: Quick Benchmark Run
```powershell
# Takes ~4 minutes
.\run_all_benchmarks.ps1 -Speed fast
```

### Task 2: Publication-Quality Run
```powershell
# Takes ~100 minutes, but very stable
.\run_all_benchmarks.ps1 -Speed thorough
```

### Task 3: View Detailed Analysis
```bash
python analyze_results.py results/benchmark_combined_2026-06-02_*.csv
```

### Task 4: Compare Two Runs (Before/After Optimization)
```python
import pandas as pd

# Load both runs
before = pd.read_csv('results/benchmark_combined_2026-06-02_120000.csv')
after = pd.read_csv('results/benchmark_combined_2026-06-02_150000.csv')

# Merge and compare
comparison = pd.merge(before, after, on='name', suffixes=('_before', '_after'))

# Calculate speedup
comparison['speedup'] = comparison['time_before'] / comparison['time_after']

# Show biggest improvements
print(comparison.nlargest(10, 'speedup')[['name', 'speedup']])
```

### Task 5: Generate CSV with Different Time Settings
```powershell
# Fast (for development)
.\run_all_benchmarks.ps1 -Speed fast

# Normal (for analysis)
.\run_all_benchmarks.ps1 -Speed normal

# Thorough (for papers/publications)
.\run_all_benchmarks.ps1 -Speed thorough
```

---

## Understanding the Output

### File Naming
```
benchmark_original_2026-06-02_HHMMSS.csv    ← 15 original benchmarks
benchmark_advanced_2026-06-02_HHMMSS.csv    ← 33 advanced benchmarks
benchmark_combined_2026-06-02_HHMMSS.csv    ← All merged together
analysis_2026-06-02_HHMMSS.txt              ← Summary report
```

### CSV Columns
```
name           Benchmark name (e.g., "BM_Addition_100x100")
time           Execution time (e.g., "3032 ns")
cpu            CPU time consumed
iterations     Number of iterations run
```

### Analysis Report Contents
```
SUMMARY STATISTICS
  - Total benchmarks
  - Timing ranges (min/max/avg/median)

CATEGORY ANALYSIS
  - Grouped by type (Scaling, Multiplication, etc.)
  - Statistics for each category

V2 PERFORMANCE (Move Semantics)
  - Copy constructor time
  - Move constructor time
  - Speedup percentage

V3 PERFORMANCE (Compound Operators)
  - Free function (+) time
  - In-place operator (+=) time
  - Speedup at different sizes

SCALING ANALYSIS
  - Matrix size progression
  - Time per element
  - O(n²) verification
```

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| "Executable not found" | Check if `bin\matrix_google_benchmark.exe` exists |
| "Script disabled" (PowerShell) | Run `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser` |
| Results CSV is empty | Benchmarks may still be compiling, wait and try again |
| Python error: "No module named csv" | Use Python 3.6+ (csv is built-in) |
| Permission denied (Bash) | Run `chmod +x run_benchmarks.sh` |
| Takes too long | Use `-Speed fast` for quick feedback |

---

## Typical Output Example

### CSV Output
```csv
name,time,cpu,iterations
BM_Addition_100x100,3032 ns,2982 ns,235789
BM_Multiply_100x100,418337 ns,386757 ns,202
BM_InPlace_Chained_100x100,5787 ns,6256 ns,44957
BM_FreeFunc_vs_InPlace_100x100_Free,3546 ns,3893 ns,20070
BM_FreeFunc_vs_InPlace_100x100_InPlace,2947 ns,3836 ns,44800
```

### Analysis Output
```
CATEGORY ANALYSIS

Category: V2: Move Constructor
  Benchmarks: 1
  Min:        689 ns
  Max:        689 ns
  Mean:       689 ns
  Median:     689 ns

Category: V2: Copy Constructor
  Benchmarks: 1
  Min:        1351 ns
  Max:        1351 ns
  Mean:       1351 ns
  Median:     1351 ns

✓ SPEEDUP: 1.96×
✓ IMPROVEMENT: 49.0%
```

---

## Advanced: Batch Processing

### Run Multiple Times and Compare
```powershell
# Day 1 baseline
.\run_all_benchmarks.ps1 -Speed normal
cp results\benchmark_combined_*.csv baseline.csv

# [Make optimizations...]

# Day 2 after changes
.\run_all_benchmarks.ps1 -Speed normal

# Compare
python compare_benchmarks.py baseline.csv results\benchmark_combined_*.csv
```

### Batch Run with Different Settings
```powershell
foreach ($speed in "fast", "normal", "thorough") {
    Write-Host "Running $speed..."
    .\run_all_benchmarks.ps1 -Speed $speed
    Start-Sleep -Seconds 5
}
```

### Merge Multiple Results
```python
import pandas as pd
import glob

# Load all result files
files = glob.glob('results/benchmark_combined_*.csv')
dfs = [pd.read_csv(f) for f in files]

# Add timestamp
for i, df in enumerate(dfs):
    df['run_number'] = i + 1

# Combine
combined = pd.concat(dfs, ignore_index=True)
combined.to_csv('all_results_combined.csv', index=False)

# Compare runs
print(combined.groupby('name')['time'].describe())
```

---

## Files Location

```
Project/
├── run_all_benchmarks.ps1        ← PowerShell script (Windows)
├── run_benchmarks.py             ← Python script (any platform)
├── run_benchmarks.sh             ← Bash script (Linux/macOS)
├── analyze_results.py            ← Analysis script
├── BENCHMARK_SCRIPTS_README.md   ← Full documentation
├── BENCHMARK_CSV_QUICKSTART.md   ← This file
│
├── results/                       ← Generated by scripts
│   ├── benchmark_original_*.csv
│   ├── benchmark_advanced_*.csv
│   ├── benchmark_combined_*.csv
│   └── analysis_*.txt
│
├── bin/
│   ├── matrix_google_benchmark.exe
│   └── matrix_benchmark_advanced.exe
│
└── code/
    ├── matrix.cpp
    ├── matrix.h
    └── matrix_benchmark_advanced.cpp
```

---

## Next Steps

1. ✅ **Run benchmarks**
   ```powershell
   .\run_all_benchmarks.ps1 -Speed normal
   ```

2. ✅ **Analyze results**
   ```bash
   python analyze_results.py results/benchmark_combined_*.csv
   ```

3. ✅ **View analysis**
   ```powershell
   cat results\analysis_*.txt
   ```

4. ✅ **Open in Excel**
   - Double-click `results/benchmark_combined_*.csv`
   - Create charts (time vs size, free vs in-place, etc.)

5. ✅ **Extract insights**
   - Look for V2 speedup (move semantics benefit)
   - Look for V3 speedup (in-place operators benefit)
   - Verify scaling is O(n²) for element-wise operations
   - Identify cache effects at large sizes (500×500+)

---

## Performance Tips

| Mode | Time | Use When |
|------|------|----------|
| **fast** | ~4 min | Quick feedback, testing |
| **normal** | ~25 min | Analysis, daily use |
| **thorough** | ~100 min | Publication, archiving |

**Recommendation:** Start with `fast`, use `normal` for analysis, use `thorough` for final publications.

---

## One-Liner Examples

### Full workflow in one command (Windows)
```powershell
.\run_all_benchmarks.ps1 -Speed normal; python analyze_results.py results/benchmark_combined_*.csv
```

### Full workflow in one command (Linux/macOS)
```bash
./run_benchmarks.sh normal && python analyze_results.py results/benchmark_combined_*.csv
```

### Open results in Excel immediately after
```powershell
.\run_all_benchmarks.ps1 -Speed normal; Start-Process (Get-ChildItem results\benchmark_combined_*.csv | Select-Object -Last 1).FullName
```

---

## What Success Looks Like

✅ Scripts run without errors  
✅ CSV files created in `results/` directory  
✅ Analysis shows V2 speedup (1.96× expected)  
✅ Analysis shows V3 speedup (1.27-2.75× expected)  
✅ Scaling verification shows O(n²) for element-wise  
✅ Scaling verification shows O(n³) for multiplication  

---

## Support

- **Full Documentation:** See [BENCHMARK_SCRIPTS_README.md](BENCHMARK_SCRIPTS_README.md)
- **Troubleshooting:** See BENCHMARK_SCRIPTS_README.md § Troubleshooting
- **Examples:** See this file § Advanced section

---

**Ready to benchmark!** 🚀

Pick your platform and run:
- **Windows:** `.\run_all_benchmarks.ps1 -Speed normal`
- **Python:** `python run_benchmarks.py`
- **Linux/macOS:** `./run_benchmarks.sh normal`

Then analyze results: `python analyze_results.py results/benchmark_combined_*.csv`
