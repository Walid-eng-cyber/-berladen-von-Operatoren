# Google Benchmark Setup Guide

This guide explains how to set up Google Benchmark for the Matrix project on Windows.

## What is Google Benchmark?

Google Benchmark is a C++ library for measuring small code snippets. It provides:
- Automatic iteration count based on time
- Better statistical analysis
- Command-line filtering and customization
- More professional reporting

---

## Option 1: Manual Setup (Recommended for Windows with LLVM)

### Step 1: Download Google Benchmark

```powershell
# Choose a directory (e.g., C:\benchmark)
cd C:\
git clone https://github.com/google/benchmark.git
cd benchmark
```

### Step 2: Build Google Benchmark

Using CMake with MinGW-compatible compiler:

```powershell
# Create build directory
mkdir build
cd build

# Configure CMake for clang
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release ..

# Build
cmake --build . --config Release

# Install (optional)
cmake --install . --prefix C:\benchmark\install
```

**Note:** If you get CMake errors, ensure you have:
- CMake installed
- clang++ in PATH
- Git installed

### Step 3: Compile Matrix Benchmarks

```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# Link with Google Benchmark library
clang++ -std=c++17 -O3 -Icode -IC:\benchmark\include code\matrix.cpp code\matrix_google_benchmark.cpp `
    -L C:\benchmark\build\src -l benchmark -l pthread -o bin\matrix_google_benchmark.exe
```

### Step 4: Run

```powershell
.\bin\matrix_google_benchmark.exe
```

---

## Option 2: Using vcpkg (Windows Package Manager)

### Install vcpkg (if not already installed):

```powershell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### Install Google Benchmark:

```powershell
.\vcpkg install benchmark:x64-windows
```

### Compile:

```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

clang++ -std=c++17 -O3 -Icode code\matrix.cpp code\matrix_google_benchmark.cpp `
    -I "path\to\vcpkg\installed\x64-windows\include" `
    -L "path\to\vcpkg\installed\x64-windows\lib" `
    -l benchmark -l pthread -o bin\matrix_google_benchmark.exe
```

---

## Option 3: Pre-compiled Binaries (Easiest)

If you have access to pre-built binaries:

1. Download from: https://github.com/google/benchmark/releases
2. Extract to `C:\benchmark`
3. Use same compilation as Option 1, Step 3

---

## Option 4: Docker (No Local Installation)

```powershell
# Create Dockerfile with Google Benchmark pre-installed
# Then build and run benchmarks in container
```

---

## Quick Troubleshooting

### Error: "benchmark.h: No such file or directory"
- Ensure benchmark is installed and include path is correct
- Check `-I C:\path\to\benchmark\include`

### Error: "undefined reference to benchmark::..."
- Ensure library path is correct with `-L`
- Ensure library name is correct with `-l benchmark`
- May need `-l pthread` for pthreads

### CMake not found
```powershell
choco install cmake -y
# OR download from https://cmake.org/download/
```

### clang++ not in PATH
```powershell
$env:PATH = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH
```

---

## Running Google Benchmarks

### Basic run:
```powershell
.\bin\matrix_google_benchmark.exe
```

### Run specific benchmark:
```powershell
.\bin\matrix_google_benchmark.exe --benchmark_filter=Addition
```

### Run with specific time:
```powershell
.\bin\matrix_google_benchmark.exe --benchmark_min_time=2.0
```

### Export results to JSON:
```powershell
.\bin\matrix_google_benchmark.exe --benchmark_out=results.json --benchmark_out_format=json
```

### Show all available benchmarks:
```powershell
.\bin\matrix_google_benchmark.exe --help
```

---

## Complete Setup Script

Save this as `setup_benchmark.ps1`:

```powershell
# Google Benchmark Setup Script for Windows

$BenchmarkDir = "C:\benchmark"
$LLVMPath = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin"
$ProjectDir = "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# Setup environment
$env:PATH = "$LLVMPath;" + $env:PATH

Write-Host "Installing Google Benchmark..." -ForegroundColor Green

# Clone Google Benchmark if not present
if (-not (Test-Path $BenchmarkDir)) {
    Write-Host "Cloning Google Benchmark..."
    git clone https://github.com/google/benchmark.git $BenchmarkDir
}

# Build Google Benchmark
cd "$BenchmarkDir"
if (-not (Test-Path "build")) {
    mkdir build
}
cd build

Write-Host "Building Google Benchmark (this may take 2-5 minutes)..." -ForegroundColor Yellow
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Compile Matrix benchmarks
Write-Host "Compiling Matrix benchmarks..." -ForegroundColor Green
cd $ProjectDir

clang++ -std=c++17 -O3 -Icode -I"$BenchmarkDir\include" code\matrix.cpp code\matrix_google_benchmark.cpp `
    -L "$BenchmarkDir\build\src" -l benchmark -l pthread -o bin\matrix_google_benchmark.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Successfully compiled!" -ForegroundColor Green
    Write-Host "`nRun benchmarks with:" -ForegroundColor Cyan
    Write-Host "  .\bin\matrix_google_benchmark.exe" -ForegroundColor Cyan
} else {
    Write-Host "✗ Compilation failed!" -ForegroundColor Red
}
```

Run it:
```powershell
.\setup_benchmark.ps1
```

---

## What's Different Between std::chrono vs Google Benchmark?

### std::chrono (Simple Version)
- ✓ No dependencies
- ✓ Easy to understand
- ✓ Good for quick benchmarking
- ✗ Must manage iteration count manually
- ✗ Less statistical analysis

### Google Benchmark
- ✓ Automatic iteration count
- ✓ Better statistical analysis
- ✓ Professional reporting
- ✓ Command-line filtering
- ✓ JSON export
- ✗ Requires installation
- ✗ More complex setup

---

## Expected Output

```
Benchmark                               Time             CPU   Iterations
────────────────────────────────────────────────────────────────────────
BM_Addition_100x100                  3.45 us         3.45 us       202813
BM_Subtraction_100x100              20.89 us        20.89 us        33500
BM_Multiplication_50x50              49.55 us        49.55 us        14108
BM_InPlace_Addition_100x100           1.78 us         1.78 us       392859
BM_Chained_Operations               87.12 us        87.12 us         8036
BM_Copy_Constructor_100x100           1.21 us         1.21 us       578432
BM_Move_Constructor_100x100           0.03 us         0.03 us     238095238
```

---

## Next Steps

1. Choose an installation option (1-4)
2. Follow the setup steps
3. Run the benchmarks
4. Compare results with std::chrono version

Prefer Option 1 (Manual) or Option 2 (vcpkg) on Windows with LLVM.

---

## References

- Google Benchmark: https://github.com/google/benchmark
- CMake: https://cmake.org
- vcpkg: https://github.com/Microsoft/vcpkg
