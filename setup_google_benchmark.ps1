#!/usr/bin/env powershell

<#
.SYNOPSIS
    Automated Google Benchmark Setup for Matrix Project
    
.DESCRIPTION
    This script downloads, builds, and compiles Google Benchmark
    for use with the Matrix benchmarking suite.
    
.EXAMPLE
    .\setup_google_benchmark.ps1
    
.NOTES
    Requires: Git, CMake, Clang++
    Time: ~5 minutes for first build
#>

param(
    [string]$BenchmarkDir = "C:\benchmark",
    [string]$LLVMPath = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin",
    [string]$ProjectDir = "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
)

# ============================================================
# Configuration
# ============================================================

Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║     Google Benchmark Setup for Matrix Project             ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

Write-Host "Configuration:" -ForegroundColor Yellow
Write-Host "  Benchmark Dir: $BenchmarkDir"
Write-Host "  LLVM Path:     $LLVMPath"
Write-Host "  Project Dir:   $ProjectDir"
Write-Host ""

# ============================================================
# Checks
# ============================================================

Write-Host "Checking prerequisites..." -ForegroundColor Yellow

# Check Git
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "✗ Git not found in PATH" -ForegroundColor Red
    Write-Host "  Install from: https://git-scm.com/download/win" -ForegroundColor Gray
    exit 1
}
Write-Host "✓ Git found" -ForegroundColor Green

# Check CMake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "✗ CMake not found in PATH" -ForegroundColor Red
    Write-Host "  Install with: choco install cmake -y" -ForegroundColor Gray
    Write-Host "  Or download from: https://cmake.org/download/" -ForegroundColor Gray
    exit 1
}
Write-Host "✓ CMake found" -ForegroundColor Green

# Check Clang++
if (-not (Test-Path "$LLVMPath\clang++.exe")) {
    Write-Host "✗ Clang++ not found at $LLVMPath" -ForegroundColor Red
    Write-Host "  Check LLVM installation" -ForegroundColor Gray
    exit 1
}
Write-Host "✓ Clang++ found" -ForegroundColor Green

# ============================================================
# Download Google Benchmark
# ============================================================

Write-Host ""
Write-Host "Setting up Google Benchmark..." -ForegroundColor Yellow

if (Test-Path $BenchmarkDir) {
    Write-Host "  Google Benchmark directory exists" -ForegroundColor Gray
    $UpdateRepo = Read-Host "  Update existing repository? (y/n)" 
    if ($UpdateRepo -eq "y") {
        Write-Host "  Updating repository..."
        cd $BenchmarkDir
        git pull origin main
    }
} else {
    Write-Host "  Cloning Google Benchmark repository..."
    git clone https://github.com/google/benchmark.git $BenchmarkDir
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Failed to clone repository" -ForegroundColor Red
        exit 1
    }
}

# ============================================================
# Build Google Benchmark
# ============================================================

Write-Host ""
Write-Host "Building Google Benchmark (this may take 2-5 minutes)..." -ForegroundColor Yellow

cd $BenchmarkDir

# Create build directory
if (-not (Test-Path "build")) {
    mkdir build | Out-Null
}

cd build

# Configure with CMake
Write-Host "  Configuring with CMake..."
cmake -G "Unix Makefiles" `
    -DCMAKE_CXX_COMPILER=clang++ `
    -DCMAKE_C_COMPILER=clang `
    -DCMAKE_BUILD_TYPE=Release `
    -DBENCHMARK_ENABLE_TESTING=OFF `
    .. 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ CMake configuration failed" -ForegroundColor Red
    exit 1
}
Write-Host "  ✓ CMake configuration succeeded" -ForegroundColor Green

# Build
Write-Host "  Building library..."
cmake --build . --config Release 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ Build failed" -ForegroundColor Red
    exit 1
}
Write-Host "  ✓ Build succeeded" -ForegroundColor Green

# ============================================================
# Compile Matrix Benchmarks
# ============================================================

Write-Host ""
Write-Host "Compiling Matrix benchmarks..." -ForegroundColor Yellow

cd $ProjectDir

# Set PATH for clang++
$env:PATH = "$LLVMPath;" + $env:PATH

# Build command
$BuildCmd = @(
    "clang++",
    "-std=c++17",
    "-O3",
    "-Icode",
    "-I`"$BenchmarkDir\include`"",
    "code\matrix.cpp",
    "code\matrix_google_benchmark.cpp",
    "-L`"$BenchmarkDir\build\src`"",
    "-lbenchmark",
    "-lpthread",
    "-o",
    "bin\matrix_google_benchmark.exe"
) -join " "

Write-Host "  Command: $BuildCmd" -ForegroundColor Gray

Invoke-Expression $BuildCmd 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ Compilation failed" -ForegroundColor Red
    Write-Host "  Trying alternative linking..." -ForegroundColor Yellow
    
    # Alternative with static linking
    clang++ -std=c++17 -O3 -Icode -I"$BenchmarkDir\include" `
        code\matrix.cpp code\matrix_google_benchmark.cpp `
        "$BenchmarkDir\build\src\libbenchmark.a" -lpthread `
        -o bin\matrix_google_benchmark.exe 2>&1
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Alternative compilation also failed" -ForegroundColor Red
        exit 1
    }
}

if (Test-Path "bin\matrix_google_benchmark.exe") {
    Write-Host "✓ Compilation succeeded" -ForegroundColor Green
} else {
    Write-Host "✗ Executable not created" -ForegroundColor Red
    exit 1
}

# ============================================================
# Success
# ============================================================

Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║              Setup Completed Successfully! ✓               ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""

Write-Host "Run benchmarks with:" -ForegroundColor Cyan
Write-Host "  cd $ProjectDir" -ForegroundColor White
Write-Host "  .\bin\matrix_google_benchmark.exe" -ForegroundColor White
Write-Host ""

Write-Host "Common commands:" -ForegroundColor Cyan
Write-Host "  # Run all benchmarks" -ForegroundColor Gray
Write-Host "  .\bin\matrix_google_benchmark.exe" -ForegroundColor White
Write-Host ""
Write-Host "  # Run specific benchmark" -ForegroundColor Gray
Write-Host "  .\bin\matrix_google_benchmark.exe --benchmark_filter=Addition" -ForegroundColor White
Write-Host ""
Write-Host "  # Show help" -ForegroundColor Gray
Write-Host "  .\bin\matrix_google_benchmark.exe --help" -ForegroundColor White
Write-Host ""
Write-Host "  # Export to JSON" -ForegroundColor Gray
Write-Host "  .\bin\matrix_google_benchmark.exe --benchmark_out=results.json --benchmark_out_format=json" -ForegroundColor White
Write-Host ""

Write-Host "Google Benchmark location: $BenchmarkDir" -ForegroundColor Gray
Write-Host "Include path:              $BenchmarkDir\include" -ForegroundColor Gray
Write-Host "Library path:              $BenchmarkDir\build\src" -ForegroundColor Gray
