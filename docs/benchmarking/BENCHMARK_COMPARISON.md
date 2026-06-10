# Google Benchmark vs std::chrono Comparison

## Quick Summary

| Feature | std::chrono | Google Benchmark |
|---------|-----------|------------------|
| Setup | None (built-in) | 5-10 minutes |
| Dependencies | None | CMake, Git |
| Learning curve | Very easy | Moderate |
| Statistical analysis | Basic | Advanced |
| Automatic tuning | Manual iterations | Automatic |
| Report format | Text | Text, JSON, CSV |
| Command-line options | None | Extensive |
| Best for | Quick tests | Production use |

---

## When to Use Each

### Use std::chrono (benchmark.h)
```powershell
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe
.\bin\matrix_benchmark.exe
```

**When:**
- ✓ Quick performance checks
- ✓ No setup time wanted
- ✓ Simple benchmarking
- ✓ Just need approximate numbers
- ✓ Comparing before/after changes quickly

**Pros:**
- Works immediately
- No installation
- Easy to modify
- Good for development

**Cons:**
- Manual iteration count tuning
- Less sophisticated analysis
- No command-line filtering
- Basic reporting

---

### Use Google Benchmark
```powershell
.\setup_google_benchmark.ps1
.\bin\matrix_google_benchmark.exe
```

**When:**
- ✓ Production benchmarking
- ✓ Publishing results
- ✓ Statistical analysis needed
- ✓ Comparing multiple runs
- ✓ Reproducible results

**Pros:**
- Professional reporting
- Automatic iteration tuning
- Advanced statistics
- Export to JSON/CSV
- Command-line filtering
- Widely recognized

**Cons:**
- Setup required
- More complex code
- Larger binary

---

## Running Both

### Setup
```powershell
# Set environment once
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# Build both versions
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe
.\setup_google_benchmark.ps1
```

### Run both
```powershell
# Simple version (instant)
Write-Host "=== Simple Benchmark (std::chrono) ===" -ForegroundColor Green
.\bin\matrix_benchmark.exe

# Professional version (takes longer)
Write-Host "`n=== Google Benchmark ===" -ForegroundColor Green
.\bin\matrix_google_benchmark.exe
```

---

## Comparing Output

### std::chrono Output
```
============================================================
  Matrix Addition (A + B)
============================================================
  Iterations:             1000
  Total Time:         4.549000 ms

  Min:                0.002500 ms
  Max:                0.262400 ms
  Mean:               0.004549 ms
  Median:             0.002700 ms
  StdDev:             0.011260 ms
============================================================
```

### Google Benchmark Output
```
Benchmark                    Time             CPU   Iterations
──────────────────────────────────────────────────────────────
BM_Addition_100x100       4.55 us         4.55 us       200000
```

**Google Benchmark advantages:**
- Automatic iteration count (200,000 vs 1000)
- Consistent time unit (microseconds)
- CPU time separate from wall time
- Side-by-side comparison format
- Much more stable statistics

---

## Code Comparison

### std::chrono (Simple)
```cpp
#include "benchmark.h"

Benchmark timer;
for (int i = 0; i < 1000; ++i) {
    timer.start();
    Matrix result = A + B;
    timer.stop();
}
timer.report("Addition");
```

**Pros:** Clear, simple, self-documenting  
**Cons:** Need to choose iteration count (1000 might not be enough)

### Google Benchmark (Professional)
```cpp
#include <benchmark/benchmark.h>

static void BM_Addition(benchmark::State& state) {
    Matrix A(100, 100), B(100, 100);
    for (auto _ : state) {
        Matrix result = A + B;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition);
BENCHMARK_MAIN();
```

**Pros:** Automatic tuning, prevents compiler optimizations  
**Cons:** More boilerplate, requires library

---

## Statistical Analysis

### std::chrono Provides
- Min, Max, Mean, Median, StdDev
- Total time
- Raw measurements

### Google Benchmark Provides
- All of above automatically
- Better outlier handling
- Iteration count optimization
- Confidence intervals (optional)
- Regression detection

---

## Performance Impact

Both benchmarking approaches add overhead, but it's negligible:

```
Actual operation time:       ~0.005 ms
std::chrono overhead:        ~0.0001 ms (2%)
Google Benchmark overhead:   ~0.0001 ms (2%)
```

The difference is less than 2% - both are fine!

---

## Choosing a Version: Decision Tree

```
Do you need production-grade results?
├─ YES → Use Google Benchmark
│   └─ Can you spend 5-10 min setting up?
│       ├─ YES → Run setup_google_benchmark.ps1
│       └─ NO → Use std::chrono
│
└─ NO → Use std::chrono (instant gratification!)
    └─ Want faster iteration?
        └─ YES → Google Benchmark (auto-tunes)
        └─ NO → std::chrono is fine
```

---

## Hybrid Approach (Recommended)

1. **Development:** Use std::chrono for quick feedback
   ```powershell
   .\bin\matrix_benchmark.exe
   ```

2. **Before commit:** Use Google Benchmark for stable numbers
   ```powershell
   .\bin\matrix_google_benchmark.exe
   ```

3. **Publication:** Export Google Benchmark JSON
   ```powershell
   .\bin\matrix_google_benchmark.exe --benchmark_out=results.json --benchmark_out_format=json
   ```

---

## Example Workflow

### Quick Test (30 seconds)
```powershell
# Make a change to matrix.cpp
# Quick test
.\bin\matrix_benchmark.exe
# Looks good? Commit!
```

### Before Committing (2 minutes)
```powershell
# Run Google Benchmark for final verification
.\bin\matrix_google_benchmark.exe
# Performance looks stable?
git commit
```

### Publishing Results (5 minutes)
```powershell
# Export for documentation
.\bin\matrix_google_benchmark.exe --benchmark_out=results.json --benchmark_out_format=json
# Add to commit/PR
```

---

## Frequently Asked Questions

**Q: Which is faster?**  
A: They measure speed, not add significant overhead. Both are fine.

**Q: Which should I use for teaching?**  
A: std::chrono! It's simpler and easier to understand.

**Q: Which for research paper?**  
A: Google Benchmark! More professional and reproducible.

**Q: Can I use both?**  
A: Yes! They can coexist. Use both for different purposes.

**Q: Is Google Benchmark worth the setup?**  
A: Yes, if you plan to:
- Share results with others
- Track performance over time
- Need statistical confidence
- Compare multiple implementations

**Q: Can I uninstall if I don't like it?**  
A: Yes, just delete `C:\benchmark` folder and binary. No system changes.

---

## Installation Verification

### Verify std::chrono works:
```powershell
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\test.exe
.\bin\test.exe
```

Expected: Output table with statistics

### Verify Google Benchmark works:
```powershell
.\bin\matrix_google_benchmark.exe
```

Expected: Output with benchmark names and times

---

## Next Steps

1. **Try std::chrono first** (immediate, no setup)
   ```powershell
   .\bin\matrix_benchmark.exe
   ```

2. **If you like it, try Google Benchmark** (better stats)
   ```powershell
   .\setup_google_benchmark.ps1
   .\bin\matrix_google_benchmark.exe
   ```

3. **Pick your favorite** for regular use

4. **Use both** in your workflow if preferred

---

**Happy benchmarking!** 🎯
