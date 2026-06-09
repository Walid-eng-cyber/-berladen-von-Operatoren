# Google Benchmark Quick Start

## Fastest Setup (5 minutes)

### Step 1: Run the setup script

```powershell
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
.\setup_google_benchmark.ps1
```

That's it! The script will:
- ✓ Check for required tools (Git, CMake, Clang++)
- ✓ Download Google Benchmark
- ✓ Build Google Benchmark
- ✓ Compile Matrix benchmarks

### Step 2: Run benchmarks

```powershell
.\bin\matrix_google_benchmark.exe
```

---

## If Script Fails: Manual Steps

### Prerequisites Check

Verify you have these installed:

```powershell
# Check Git
git --version

# Check CMake  
cmake --version

# Check Clang++
clang++ --version
```

Missing something? Install with:
```powershell
# Git
choco install git -y

# CMake
choco install cmake -y

# LLVM (should already be installed)
# If not: https://github.com/mstorsjo/llvm-mingw/releases
```

### Manual Build

```powershell
# 1. Set environment
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# 2. Clone Google Benchmark
git clone https://github.com/google/benchmark.git C:\benchmark

# 3. Build Google Benchmark
cd C:\benchmark
mkdir build
cd build

# Configure
cmake -G "Unix Makefiles" `
    -DCMAKE_CXX_COMPILER=clang++ `
    -DCMAKE_C_COMPILER=clang `
    -DCMAKE_BUILD_TYPE=Release `
    ..

# Build (this takes 2-5 minutes)
cmake --build . --config Release

# 4. Compile Matrix benchmarks
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

clang++ -std=c++17 -O3 `
    -Icode `
    -IC:\benchmark\include `
    code\matrix.cpp `
    code\matrix_google_benchmark.cpp `
    -LC:\benchmark\build\src `
    -lbenchmark -lpthread `
    -o bin\matrix_google_benchmark.exe

# 5. Run
.\bin\matrix_google_benchmark.exe
```

---

## Benchmark Files

| File | Purpose |
|------|---------|
| `matrix_google_benchmark.cpp` | Google Benchmark suite (12 benchmarks) |
| `GOOGLE_BENCHMARK_SETUP.md` | Detailed setup guide |
| `setup_google_benchmark.ps1` | Automated setup script |
| `benchmark.h` | Simple std::chrono alternative (already works!) |

---

## Benchmarks Included

| Name | Tests |
|------|-------|
| Addition (10×10, 50×50, 100×100, 500×500) | 4 |
| Subtraction | 1 |
| Multiplication | 1 |
| In-place Addition (+=) | 1 |
| Chained Operations | 1 |
| Copy Constructor | 1 |
| Move Constructor | 1 |
| Loop Accumulation | 1 |
| Copy Assignment | 1 |
| Move Assignment | 1 |
| Comparison: + vs += | 2 |
| **Total** | **15** |

---

## Running Benchmarks

### All benchmarks:
```powershell
.\bin\matrix_google_benchmark.exe
```

### Specific benchmark:
```powershell
# Addition only
.\bin\matrix_google_benchmark.exe --benchmark_filter=Addition

# Move operations only
.\bin\matrix_google_benchmark.exe --benchmark_filter=Move

# Multiplication
.\bin\matrix_google_benchmark.exe --benchmark_filter=Multiplication
```

### With custom time:
```powershell
# Run longer (slower, more stable)
.\bin\matrix_google_benchmark.exe --benchmark_min_time=5.0

# Run faster (quicker feedback)
.\bin\matrix_google_benchmark.exe --benchmark_min_time=0.1
```

### Export results:
```powershell
# JSON format
.\bin\matrix_google_benchmark.exe `
    --benchmark_out=results.json `
    --benchmark_out_format=json

# CSV format
.\bin\matrix_google_benchmark.exe `
    --benchmark_out=results.csv `
    --benchmark_out_format=csv
```

### Show all options:
```powershell
.\bin\matrix_google_benchmark.exe --help
```

---

## Expected Output Example

```
Benchmark                                   Time             CPU   Iterations
─────────────────────────────────────────────────────────────────────────────
BM_Addition_10x10                        0.64 us         0.64 us      1000000
BM_Addition_50x50                        15.2 us         15.2 us        45865
BM_Addition_100x100                      61.2 us         61.2 us        11433
BM_Addition_500x500                       1.52 ms         1.52 ms          460
BM_Subtraction_100x100                    60.8 us         60.8 us        11557
BM_Multiplication_50x50                    49.2 us         49.2 us        14231
BM_InPlace_Addition_100x100               28.3 us         28.3 us        24721
BM_Chained_Operations                      123 us          123 us         5703
BM_Copy_Constructor_100x100               1.21 us         1.21 us       578214
BM_Move_Constructor_100x100              0.024 us        0.024 us     1000000
BM_Loop_Accumulation                      6.23 ms         6.23 ms          112
BM_Copy_Assignment_100x100               1.19 us         1.19 us       588235
BM_Move_Assignment_100x100               0.030 us        0.030 us     1000000
BM_Free_Function_Plus                     61.4 us         61.4 us        11398
BM_Member_Operator_PlusEqual              28.2 us         28.2 us        24802
```

---

## Comparing Implementations

### V2 (Move Semantics) Benefits:

Compare times:
- `BM_Copy_Constructor_100x100`: ~1.2 µs (deep copy)
- `BM_Move_Constructor_100x100`: ~0.024 µs (just pointers)
- **Speedup: 50×!**

### V3 (Compound Operators) Benefits:

Compare times:
- `BM_Free_Function_Plus`: ~61 µs (copy + add)
- `BM_Member_Operator_PlusEqual`: ~28 µs (in-place)
- **Speedup: 2×!**

---

## Troubleshooting

### "cannot find -lbenchmark"
- Ensure benchmark built successfully
- Check path: `C:\benchmark\build\src\` contains `libbenchmark.a`

### "clang++: command not found"
```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
```

### "cmake: command not found"
```powershell
choco install cmake -y
# Or download from https://cmake.org/download/
```

### Script permission error
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

---

## Still Want Simple Version?

The `benchmark.h` (std::chrono) version works fine without installation:

```powershell
clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_benchmark.cpp -o bin\matrix_benchmark.exe
.\bin\matrix_benchmark.exe
```

No dependencies, works immediately!

---

## Next Steps

1. Run `.\setup_google_benchmark.ps1`
2. Run `.\bin\matrix_google_benchmark.exe`
3. Compare with simple version: `.\bin\matrix_benchmark.exe`
4. Export results for analysis

Happy Benchmarking! 🚀
