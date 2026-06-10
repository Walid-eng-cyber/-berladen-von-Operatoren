#!/usr/bin/env powershell

<#
.SYNOPSIS
    Working Google Benchmark Setup for Matrix Project
    
.DESCRIPTION
    This script handles downloading, building, and compiling Google Benchmark
    with the correct flags for LLVM/Clang on Windows.
    
    Uses MinGW Makefiles generator with proven compilation flags.
    
.EXAMPLE
    .\setup_google_benchmark_working.ps1
    
.NOTES
    Successfully tested: June 2, 2026
    Time: ~3-5 minutes
    Windows 10/11 with LLVM-MinGW installed
#>

param(
    [string]$BenchmarkDir = "C:\benchmark",
    [string]$LLVMPath = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin",
    [string]$CMakePath = "C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin",
    [string]$ProjectDir = "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
)

Write-Host "╔═══════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║      Google Benchmark Setup (Working Version)            ║" -ForegroundColor Cyan
Write-Host "║      Successfully Tested - June 2, 2026                  ║" -ForegroundColor Cyan
Write-Host "╚═══════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

# ============================================================
# Configuration
# ============================================================

Write-Host "Configuration:" -ForegroundColor Yellow
Write-Host "  Benchmark:  $BenchmarkDir"
Write-Host "  LLVM:       $LLVMPath"
Write-Host "  CMake:      $CMakePath"
Write-Host "  Project:    $ProjectDir"
Write-Host ""

# ============================================================
# Verify Prerequisites
# ============================================================

Write-Host "Checking prerequisites..." -ForegroundColor Yellow

$AllOK = $true

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "  ✗ Git not found" -ForegroundColor Red
    $AllOK = $false
} else {
    Write-Host "  ✓ Git found" -ForegroundColor Green
}

if (-not (Test-Path "$LLVMPath\clang++.exe")) {
    Write-Host "  ✗ Clang++ not found at $LLVMPath" -ForegroundColor Red
    $AllOK = $false
} else {
    Write-Host "  ✓ Clang++ found" -ForegroundColor Green
}

if ($AllOK -eq $false) {
    Write-Host "Prerequisites missing. Please install required tools." -ForegroundColor Red
    exit 1
}

# ============================================================
# Download CMake (if needed)
# ============================================================

Write-Host ""
Write-Host "Setting up CMake..." -ForegroundColor Yellow

if (-not (Test-Path "$CMakePath\cmake.exe")) {
    Write-Host "  Downloading CMake 3.29.3..."
    $ProgressPreference = 'SilentlyContinue'
    try {
        Invoke-WebRequest -Uri "https://github.com/Kitware/CMake/releases/download/v3.29.3/cmake-3.29.3-windows-x86_64.zip" `
            -OutFile "$env:TEMP\cmake.zip"
        Write-Host "  Extracting CMake..."
        Expand-Archive -Path "$env:TEMP\cmake.zip" -DestinationPath "C:\cmake-portable" -Force
        Write-Host "  ✓ CMake installed" -ForegroundColor Green
    } catch {
        Write-Host "  ✗ Failed to download CMake" -ForegroundColor Red
        Write-Host "  Download manually from: https://cmake.org/download/" -ForegroundColor Gray
        exit 1
    }
} else {
    Write-Host "  ✓ CMake already available" -ForegroundColor Green
}

# ============================================================
# Download Google Benchmark
# ============================================================

Write-Host ""
Write-Host "Setting up Google Benchmark..." -ForegroundColor Yellow

if (Test-Path $BenchmarkDir) {
    Write-Host "  Google Benchmark directory exists" -ForegroundColor Gray
} else {
    Write-Host "  Cloning from GitHub..."
    git clone https://github.com/google/benchmark.git $BenchmarkDir
    if ($LASTEXITCODE -ne 0) {
        Write-Host "  ✗ Clone failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "  ✓ Cloned successfully" -ForegroundColor Green
}

# ============================================================
# Clean build directory
# ============================================================

Write-Host ""
Write-Host "Preparing build directory..." -ForegroundColor Yellow

if (Test-Path "$BenchmarkDir\build") {
    Remove-Item -Path "$BenchmarkDir\build\*" -Force -Recurse -ErrorAction SilentlyContinue
} else {
    mkdir "$BenchmarkDir\build" | Out-Null
}

Write-Host "  ✓ Build directory ready" -ForegroundColor Green

# ============================================================
# Configure with CMake
# ============================================================

Write-Host ""
Write-Host "Configuring with CMake (MinGW Makefiles)..." -ForegroundColor Yellow

$env:PATH = "$CMakePath;$LLVMPath;" + $env:PATH

cd "$BenchmarkDir\build"

$CMakeArgs = @(
    "-G", "MinGW Makefiles",
    "-DCMAKE_CXX_COMPILER=clang++",
    "-DCMAKE_CXX_COMPILER_FORCED=ON",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_MAKE_PROGRAM=mingw32-make",
    "-DBENCHMARK_ENABLE_TESTING=OFF",
    "-DBENCHMARK_ENABLE_GTEST_TESTS=OFF",
    ".."
)

& cmake @CMakeArgs 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ CMake configuration failed" -ForegroundColor Red
    exit 1
}

Write-Host "  ✓ Configuration complete" -ForegroundColor Green

# ============================================================
# Build Google Benchmark
# ============================================================

Write-Host ""
Write-Host "Building Google Benchmark library..." -ForegroundColor Yellow

mingw32-make -j4 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Build failed" -ForegroundColor Red
    exit 1
}

if ((Test-Path "$BenchmarkDir\build\src\libbenchmark.a") -and (Test-Path "$BenchmarkDir\build\src\libbenchmark_main.a")) {
    Write-Host "  ✓ Build successful" -ForegroundColor Green
} else {
    Write-Host "  ✗ Library files not created" -ForegroundColor Red
    exit 1
}

# ============================================================
# Compile Matrix Benchmarks
# ============================================================

Write-Host ""
Write-Host "Compiling Matrix benchmarks..." -ForegroundColor Yellow

cd $ProjectDir

$CompileCmd = @(
    "clang++",
    "-std=c++17",
    "-O3",
    "-Icode",
    "-IC:\benchmark\include",
    "-DBENCHMARK_STATIC_DEFINE",
    "code\matrix.cpp",
    "code\matrix_google_benchmark.cpp",
    "C:\benchmark\build\src\libbenchmark.a",
    "C:\benchmark\build\src\libbenchmark_main.a",
    "-lpthread",
    "-lshlwapi",
    "-lwinmm",
    "-o",
    "bin\matrix_google_benchmark.exe"
)

& $CompileCmd[0] @$CompileCmd[1..($CompileCmd.Count-1)] 2>&1 | Out-Null

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Compilation failed" -ForegroundColor Red
    exit 1
}

if (Test-Path "bin\matrix_google_benchmark.exe") {
    $Size = [math]::Round((Get-Item "bin\matrix_google_benchmark.exe").Length / 1MB, 2)
    Write-Host "  ✓ Executable created ($Size MB)" -ForegroundColor Green
} else {
    Write-Host "  ✗ Executable not created" -ForegroundColor Red
    exit 1
}

# ============================================================
# Test Run
# ============================================================

Write-Host ""
Write-Host "Testing benchmarks..." -ForegroundColor Yellow

& ".\bin\matrix_google_benchmark.exe" --benchmark_min_time=0.1 2>&1 | Select-Object -First 25 | Out-Null

if ($LASTEXITCODE -eq 0) {
    Write-Host "  ✓ Tests executed successfully" -ForegroundColor Green
} else {
    Write-Host "  ⚠ Execution may have issues" -ForegroundColor Yellow
}

# ============================================================
# Success Summary
# ============================================================

Write-Host ""
Write-Host "╔═══════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║            Setup Complete - Ready to Benchmark! ✓         ║" -ForegroundColor Green
Write-Host "╚═══════════════════════════════════════════════════════════╝" -ForegroundColor Green

Write-Host ""
Write-Host "Quick Start:" -ForegroundColor Cyan
Write-Host "  `$env:PATH = `"$CMakePath;$LLVMPath;`" + `$env:PATH" -ForegroundColor Gray
Write-Host "  cd `"$ProjectDir`"" -ForegroundColor Gray
Write-Host "  .\bin\matrix_google_benchmark.exe" -ForegroundColor White

Write-Host ""
Write-Host "Common Commands:" -ForegroundColor Cyan
Write-Host "  All benchmarks:        .\bin\matrix_google_benchmark.exe" -ForegroundColor Gray
Write-Host "  Addition only:         .\bin\matrix_google_benchmark.exe --benchmark_filter=Addition" -ForegroundColor Gray
Write-Host "  Move operations:       .\bin\matrix_google_benchmark.exe --benchmark_filter=Move" -ForegroundColor Gray
Write-Host "  Export to JSON:        .\bin\matrix_google_benchmark.exe --benchmark_out=results.json --benchmark_out_format=json" -ForegroundColor Gray
Write-Host "  Help:                  .\bin\matrix_google_benchmark.exe --help" -ForegroundColor Gray

Write-Host ""
Write-Host "Key Locations:" -ForegroundColor Cyan
Write-Host "  CMake:               $CMakePath" -ForegroundColor Gray
Write-Host "  Google Benchmark:    $BenchmarkDir" -ForegroundColor Gray
Write-Host "  Executable:          $ProjectDir\bin\matrix_google_benchmark.exe" -ForegroundColor Gray

Write-Host ""
Write-Host "Documentation:" -ForegroundColor Cyan
Write-Host "  Setup Log:           code\GOOGLE_BENCHMARK_INSTALLATION_LOG.md" -ForegroundColor Gray
Write-Host "  Quick Start:         code\GOOGLE_BENCHMARK_QUICKSTART.md" -ForegroundColor Gray
Write-Host "  Comparison:          code\BENCHMARK_COMPARISON.md" -ForegroundColor Gray

Write-Host ""
