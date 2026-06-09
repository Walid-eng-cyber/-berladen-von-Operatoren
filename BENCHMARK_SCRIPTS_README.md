# Benchmark CSV Export Scripts

**Date:** June 2, 2026  
**Status:** ✅ Ready to Use

---

## Quick Start

### Windows (PowerShell)
```powershell
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
.\run_all_benchmarks.ps1 -Speed normal
```

### Linux/macOS (Bash)
```bash
chmod +x run_benchmarks.sh
./run_benchmarks.sh normal
```

### Python (Cross-Platform)
```bash
python run_benchmarks.py
```

---

## Scripts Overview

### 1. **run_all_benchmarks.ps1** (PowerShell - Windows)

**Purpose:** Automated benchmark execution with CSV export (Windows-native)

**Features:**
- Automatic environment setup (CMake, LLVM in PATH)
- Runs both original and advanced suites
- Exports to CSV format
- Generates analysis report
- Color-coded output

**Usage:**
```powershell
.\run_all_benchmarks.ps1                  # Normal (500ms per benchmark)
.\run_all_benchmarks.ps1 -Speed fast      # Quick (50ms per benchmark)
.\run_all_benchmarks.ps1 -Speed thorough  # Detailed (2s per benchmark)
```

**Output:**
```
results/
  ├── benchmark_original_2026-06-02_HHMMSS.csv      (15 benchmarks)
  ├── benchmark_advanced_2026-06-02_HHMMSS.csv      (33 benchmarks)
  ├── benchmark_combined_2026-06-02_HHMMSS.csv      (all merged)
  └── analysis_2026-06-02_HHMMSS.txt               (summary)
```

---

### 2. **run_benchmarks.py** (Python - Cross-Platform)

**Purpose:** Cross-platform benchmark runner with analysis

**Features:**
- Works on Windows, Linux, macOS
- Automatic environment detection
- CSV parsing and analysis
- Result categorization
- Comprehensive analysis report

**Requirements:**
```bash
python >= 3.6
# No external dependencies (uses only stdlib)
```

**Usage:**
```bash
# Default (normal speed)
python run_benchmarks.py

# Fast mode (development)
python run_benchmarks.py --speed fast

# Thorough mode (publication quality)
python run_benchmarks.py --speed thorough

# Filter specific benchmarks
python run_benchmarks.py --speed normal --filter Addition
```

**Output:**
```
results/
  ├── benchmark_original_2026-06-02_HHMMSS.csv
  ├── benchmark_advanced_2026-06-02_HHMMSS.csv
  ├── benchmark_combined_2026-06-02_HHMMSS.csv
  └── analysis_2026-06-02_HHMMSS.txt
```

---

### 3. **run_benchmarks.sh** (Bash - Linux/macOS)

**Purpose:** UNIX-friendly benchmark script

**Features:**
- Portable across Linux/macOS
- Simple, readable shell script
- Efficient CSV combining
- Color output

**Usage:**
```bash
chmod +x run_benchmarks.sh
./run_benchmarks.sh fast       # 50ms per benchmark
./run_benchmarks.sh normal     # 500ms per benchmark
./run_benchmarks.sh thorough   # 2s per benchmark
```

**Requirements:**
- Bash >= 4.0
- g++ or clang++ (for compiling if needed)

---

## CSV File Format

### Column Headers
```
name                           (Benchmark name)
time                          (Execution time with unit: ns/µs/ms/s)
cpu                           (CPU time)
iterations                    (Number of iterations run)
```

### Example Rows
```
BM_Addition_100x100, 3032 ns, 2982 ns, 235789
BM_Multiply_100x100, 418337 ns, 386757 ns, 202
BM_InPlace_Chained_100x100, 5787 ns, 6256 ns, 44957
```

---

## Analysis Report Format

### Generated Report Contains

1. **Summary**
   - Total benchmarks
   - Generation timestamp

2. **By Category**
   - Count for each category:
     - Chained Operations
     - Matrix Multiplication
     - In-Place Operations
     - Free vs In-Place
     - Iterative Refinement
     - Scaling Analysis

3. **V2 Performance** (Move Semantics)
   - Copy Constructor results
   - Move Constructor results
   - Comparison

4. **V3 Performance** (Compound Operators)
   - Free function (+) results
   - In-Place operator (+=) results
   - Comparison

5. **Scaling Analysis**
   - All scaling benchmarks
   - Size progression
   - Time progression

---

## Speed Settings

| Mode | Benchmark Time | Use Case |
|------|---|---|
| **fast** | 50ms | Development, quick feedback |
| **normal** | 500ms | Standard analysis |
| **thorough** | 2s | Publication quality, long-term storage |

### Timing Examples

```
fast mode:
  Total run time: ~1-2 minutes
  Stability: Good (minimal outliers)

normal mode (default):
  Total run time: ~5-10 minutes
  Stability: Excellent (recommended)

thorough mode:
  Total run time: ~15-20 minutes
  Stability: Maximum (best for papers)
```

---

## Processing CSV Results

### Excel/Calc
1. Open `benchmark_combined_*.csv`
2. Create charts for:
   - Time vs Matrix Size (scaling analysis)
   - Free vs In-Place (V3 comparison)
   - Copy vs Move (V2 comparison)

### Python (pandas)
```python
import pandas as pd

# Load combined results
df = pd.read_csv('results/benchmark_combined_2026-06-02_HHMMSS.csv')

# Show V2 comparison
print(df[df['name'].str.contains('Constructor')])

# Show V3 comparison
print(df[df['name'].str.contains('FreeFunc|InPlace')])

# Group by category
df['category'] = df['name'].str.extract('(Chained|Multiply|InPlace|FreeFunc|Iterative|Scaling)')
print(df.groupby('category')['time'].describe())
```

### R
```r
# Load data
df <- read.csv('results/benchmark_combined_2026-06-02_HHMMSS.csv')

# Plot scaling
plot(df$name, df$time, main="Benchmark Scaling")

# Statistics
summary(df$time)
```

---

## Combining Results from Multiple Runs

### Create Longitudinal Data
```powershell
# Day 1 benchmark
.\run_all_benchmarks.ps1 -Speed normal

# Day 2 benchmark (after optimizations)
.\run_all_benchmarks.ps1 -Speed normal

# Both CSVs now in results/ with timestamps
# Import both into Excel to compare over time
```

### Python Script to Merge Multiple Runs
```python
import pandas as pd
import glob

# Load all benchmark CSVs
files = sorted(glob.glob('results/benchmark_combined_*.csv'))
dfs = [pd.read_csv(f) for f in files]

# Add run date
for i, df in enumerate(dfs):
    df['run'] = i + 1

# Combine
combined = pd.concat(dfs, ignore_index=True)
combined.to_csv('results/benchmark_all_runs_combined.csv', index=False)

# Compare runs
print(combined.groupby(['name', 'run'])['time'].mean().unstack())
```

---

## Troubleshooting

### PowerShell: "Script cannot be loaded because running scripts is disabled"
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### PowerShell: Executable not found
```powershell
# Verify compilation
Test-Path "bin\matrix_google_benchmark.exe"
Test-Path "bin\matrix_benchmark_advanced.exe"

# If missing, compile:
# (See ADVANCED_BENCHMARKING_GUIDE.md)
```

### Python: ModuleNotFoundError
```bash
# Python has no external dependencies, but ensure Python 3.6+
python --version

# Verify benchmarks exist
ls bin/matrix_*benchmark*
```

### Bash: Command not found
```bash
# Ensure execute permission
chmod +x run_benchmarks.sh

# Run with explicit bash
bash run_benchmarks.sh normal
```

### Results directory permission error
```powershell
# PowerShell
New-Item -ItemType Directory -Force -Path "results"

# Bash
mkdir -p results
chmod 755 results
```

---

## Example Workflow

### Complete Analysis Session

```powershell
# 1. Run benchmarks (normal speed, ~8 minutes)
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
.\run_all_benchmarks.ps1 -Speed normal

# 2. View analysis
cat "results\analysis_*.txt"

# 3. Open in Excel
start "results\benchmark_combined_*.csv"

# 4. Create charts in Excel:
#    - Time vs Matrix Size (scaling)
#    - Free (+) vs In-Place (+=) (V3 benefit)
#    - Copy vs Move (V2 benefit)
```

---

## Advanced Usage

### Batch Multiple Runs
```powershell
# Run 3 times and keep all results
foreach ($i in 1..3) {
    Write-Host "Run $i..."
    .\run_all_benchmarks.ps1 -Speed normal
    Start-Sleep -Seconds 5
}

# All results saved with unique timestamps
Get-ChildItem "results\benchmark_combined_*.csv"
```

### Filter Specific Benchmarks
```python
# Modify run_benchmarks.py to add filter:
# success = run_benchmark(..., filter_pattern='Addition')

python run_benchmarks.py --filter "Multiply"  # Planned feature
```

### Generate Comparison Report
```python
import pandas as pd

# Load multiple runs
run1 = pd.read_csv('results/benchmark_combined_2026-06-02_120000.csv')
run2 = pd.read_csv('results/benchmark_combined_2026-06-02_150000.csv')

# Create comparison
comparison = pd.merge(
    run1[['name', 'time']], 
    run2[['name', 'time']], 
    on='name',
    suffixes=('_before', '_after')
)

comparison['improvement_%'] = (
    (comparison['time_before'] - comparison['time_after']) / 
    comparison['time_before'] * 100
)

print(comparison[comparison['improvement_%'] > 0])  # Show speedups
```

---

## File Structure

```
Project Root/
├── run_all_benchmarks.ps1     (PowerShell - Windows)
├── run_benchmarks.py          (Python - Cross-Platform)
├── run_benchmarks.sh          (Bash - Linux/macOS)
│
├── results/                   (Created by scripts)
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
    ├── matrix_benchmark_advanced.cpp
    └── benchmark.h
```

---

## Performance Notes

### Expected Runtime
- **Fast mode** (50ms per benchmark):
  - Original (15): ~2 minutes
  - Advanced (33): ~2 minutes
  - Total: ~4 minutes

- **Normal mode** (500ms per benchmark):
  - Original (15): ~8 minutes
  - Advanced (33): ~17 minutes
  - Total: ~25 minutes

- **Thorough mode** (2s per benchmark):
  - Original (15): ~30 minutes
  - Advanced (33): ~70 minutes
  - Total: ~100 minutes (best for publication)

### Output Size
- Each CSV: 5-50 KB
- All files: ~100 KB

---

## Recommended Usage

### Development Phase
```bash
./run_benchmarks.ps1 -Speed fast    # Quick feedback
```

### Analysis Phase
```bash
./run_benchmarks.ps1 -Speed normal  # Stable results
```

### Publication Phase
```bash
./run_benchmarks.ps1 -Speed thorough  # Maximum confidence
```

---

## Next Steps

1. ✅ Run benchmarks: `.\run_all_benchmarks.ps1`
2. ✅ Review analysis: `cat results\analysis_*.txt`
3. ✅ Open CSV: Import to Excel or Python
4. ✅ Create charts: Visualize V2 vs V3 benefits
5. ✅ Compare over time: Track optimizations

---

**All scripts ready to use!** 🚀
