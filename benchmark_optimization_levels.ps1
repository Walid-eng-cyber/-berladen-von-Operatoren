# Benchmark across different optimization levels (-O0, -O2, -O3)
# Compiles and runs benchmarks at each level, saves results separately

param(
    [ValidateSet("fast", "normal", "thorough")]
    [string]$Speed = "normal"
)

# Configuration
$ClangPath = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin\clang++.exe"
$BenchmarkDir = "C:\benchmark"
$SourceDir = "code"
$BinDir = "bin"
$ObjDir = "obj"
$ResultsDir = "results"
$CxxStandard = "c++17"

# Optimization levels to test
$OptimizationLevels = @("-O0", "-O2", "-O3")

# Benchmark min time based on speed setting
$BenchmarkTimes = @{
    "fast"     = "50ms"
    "normal"   = "500ms"
    "thorough" = "2000ms"
}
$MinTime = $BenchmarkTimes[$Speed]

Write-Host "========== Multi-Optimization Benchmark Suite (-O0, -O2, -O3) ==========" -ForegroundColor Cyan
Write-Host ""
Write-Host "Speed Setting: $Speed (min time: $MinTime)" -ForegroundColor Yellow
Write-Host "Optimization Levels: $($OptimizationLevels -join ', ')" -ForegroundColor Yellow
Write-Host ""

# Set up environment
$ClangDir = Split-Path -Parent $ClangPath
$env:PATH = "$ClangDir;$BenchmarkDir\bin;$BenchmarkDir\lib;" + $env:PATH

# Verify Clang
if (-not (Test-Path $ClangPath)) {
    Write-Host "ERROR: clang++ not found at $ClangPath" -ForegroundColor Red
    Exit 1
}

Write-Host "[OK] Compiler: $ClangPath" -ForegroundColor Green
Write-Host "[OK] Google Benchmark: $BenchmarkDir" -ForegroundColor Green
Write-Host ""

# Create directories
New-Item -ItemType Directory -Force -Path $BinDir | Out-Null
New-Item -ItemType Directory -Force -Path $ObjDir | Out-Null
New-Item -ItemType Directory -Force -Path $ResultsDir | Out-Null

# Function to compile benchmarks with specific optimization
function Compile-Benchmarks {
    param(
        [string]$OptFlag
    )
    
    $OptLabel = $OptFlag -replace "^-", ""
    Write-Host "===============================================================" -ForegroundColor Cyan
    Write-Host "Compiling benchmarks with $OptFlag (O$OptLabel)" -ForegroundColor Cyan
    Write-Host "===============================================================" -ForegroundColor Cyan
    
    # Common compiler flags
    $CommonFlags = @(
        "-std=$CxxStandard",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        $OptFlag,
        "-DNDEBUG"
    )
    
    # Google Benchmark link flags
    $BenchmarkFlags = @(
        "-DBENCHMARK_STATIC_DEFINE",
        "-I$BenchmarkDir\include"
    )
    
    # Full paths to benchmark libraries
    $BenchmarkLib = "$BenchmarkDir\build\src\libbenchmark.a"
    $BenchmarkMainLib = "$BenchmarkDir\build\src\libbenchmark_main.a"
    
    $LinkFlags = @(
        $BenchmarkLib,
        $BenchmarkMainLib,
        "-lshlwapi",
        "-lwinmm",
        "-lpthread"
    )
    
    # Compile matrix.cpp
    Write-Host "  Compiling matrix.cpp..." -ForegroundColor Yellow
    $MatrixObj = "$ObjDir\matrix_${OptLabel}.obj"
    & $ClangPath $CommonFlags -c "$SourceDir\matrix.cpp" -o $MatrixObj
    if ($LASTEXITCODE -ne 0) {
        Write-Host "    [FAIL] Compilation failed!" -ForegroundColor Red
        return $false
    }
    Write-Host "    [OK] $MatrixObj" -ForegroundColor Green
    
    # Compile matrix_google_benchmark.cpp
    Write-Host "  Compiling matrix_google_benchmark.cpp..." -ForegroundColor Yellow
    $BenchOrigObj = "$ObjDir\matrix_google_benchmark_${OptLabel}.obj"
    & $ClangPath $CommonFlags $BenchmarkFlags -I"$SourceDir" -c "$SourceDir\matrix_google_benchmark.cpp" -o $BenchOrigObj
    if ($LASTEXITCODE -ne 0) {
        Write-Host "    [FAIL] Compilation failed!" -ForegroundColor Red
        return $false
    }
    Write-Host "    [OK] $BenchOrigObj" -ForegroundColor Green
    
    # Link original benchmark
    Write-Host "  Linking matrix_google_benchmark_${OptLabel}.exe..." -ForegroundColor Yellow
    $BenchOrigExe = "$BinDir\matrix_google_benchmark_${OptLabel}.exe"
    & $ClangPath $CommonFlags $BenchmarkFlags $MatrixObj $BenchOrigObj $LinkFlags -o $BenchOrigExe
    if ($LASTEXITCODE -ne 0) {
        Write-Host "    [FAIL] Linking failed!" -ForegroundColor Red
        return $false
    }
    Write-Host "    [OK] $BenchOrigExe" -ForegroundColor Green
    
    # Compile matrix_benchmark_advanced.cpp
    Write-Host "  Compiling matrix_benchmark_advanced.cpp..." -ForegroundColor Yellow
    $BenchAdvObj = "$ObjDir\matrix_benchmark_advanced_${OptLabel}.obj"
    & $ClangPath $CommonFlags $BenchmarkFlags -I"$SourceDir" -c "$SourceDir\matrix_benchmark_advanced.cpp" -o $BenchAdvObj
    if ($LASTEXITCODE -ne 0) {
        Write-Host "    [FAIL] Compilation failed!" -ForegroundColor Red
        return $false
    }
    Write-Host "    [OK] $BenchAdvObj" -ForegroundColor Green
    
    # Link advanced benchmark
    Write-Host "  Linking matrix_benchmark_advanced_${OptLabel}.exe..." -ForegroundColor Yellow
    $BenchAdvExe = "$BinDir\matrix_benchmark_advanced_${OptLabel}.exe"
    & $ClangPath $CommonFlags $BenchmarkFlags $MatrixObj $BenchAdvObj $LinkFlags -o $BenchAdvExe
    if ($LASTEXITCODE -ne 0) {
        Write-Host "    [FAIL] Linking failed!" -ForegroundColor Red
        return $false
    }
    Write-Host "    [OK] $BenchAdvExe" -ForegroundColor Green
    
    Write-Host ""
    return $true
}

# Function to run benchmarks
function Run-Benchmarks {
    param(
        [string]$OptFlag,
        [string]$ExeSuffix
    )
    
    $OptLabel = $OptFlag -replace "^-", ""
    Write-Host "---------------------------------------------------------------" -ForegroundColor Green
    Write-Host "Running benchmarks compiled with $OptFlag (O$OptLabel)" -ForegroundColor Green
    Write-Host "---------------------------------------------------------------" -ForegroundColor Green
    
    # Output file names
    $OrigCsv = "$ResultsDir\benchmark_original_O${OptLabel}_$(Get-Date -Format 'yyyyMMdd_HHmmss').csv"
    $AdvCsv = "$ResultsDir\benchmark_advanced_O${OptLabel}_$(Get-Date -Format 'yyyyMMdd_HHmmss').csv"
    
    $OrigExe = "$BinDir\matrix_google_benchmark_${ExeSuffix}.exe"
    $AdvExe = "$BinDir\matrix_benchmark_advanced_${ExeSuffix}.exe"
    
    # Run original benchmarks
    if (Test-Path $OrigExe) {
        Write-Host "`n  Running matrix_google_benchmark_${ExeSuffix}.exe..." -ForegroundColor Cyan
        Write-Host "  Saving to: $OrigCsv" -ForegroundColor Cyan
        & $OrigExe --benchmark_min_time=$MinTime --benchmark_out=$OrigCsv --benchmark_out_format=csv 2>&1 | Select-String -Pattern "Benchmark|Running" | ForEach-Object { Write-Host "    $_" }
        
        if (Test-Path $OrigCsv) {
            $Size = (Get-Item $OrigCsv).Length / 1KB
            Write-Host "  [OK] Saved ($([Math]::Round($Size, 1)) KB)" -ForegroundColor Green
        }
    }
    
    # Run advanced benchmarks
    if (Test-Path $AdvExe) {
        Write-Host "`n  Running matrix_benchmark_advanced_${ExeSuffix}.exe..." -ForegroundColor Cyan
        Write-Host "  Saving to: $AdvCsv" -ForegroundColor Cyan
        & $AdvExe --benchmark_min_time=$MinTime --benchmark_out=$AdvCsv --benchmark_out_format=csv 2>&1 | Select-String -Pattern "Benchmark|Running" | ForEach-Object { Write-Host "    $_" }
        
        if (Test-Path $AdvCsv) {
            $Size = (Get-Item $AdvCsv).Length / 1KB
            Write-Host "  [OK] Saved ($([Math]::Round($Size, 1)) KB)" -ForegroundColor Green
        }
    }
    
    Write-Host ""
}

# Main execution
$StartTime = Get-Date

# Iterate through optimization levels
foreach ($OptLevel in $OptimizationLevels) {
    $OptLabel = $OptLevel -replace "^-", ""
    
    # Compile
    $CompileSuccess = Compile-Benchmarks -OptFlag $OptLevel
    if (-not $CompileSuccess) {
        Write-Host "Skipping benchmarks for $OptLevel due to compilation failure" -ForegroundColor Red
        continue
    }
    
    # Run
    Run-Benchmarks -OptFlag $OptLevel -ExeSuffix $OptLabel
}

# Summary
$EndTime = Get-Date
$Duration = $EndTime - $StartTime

Write-Host "========== Benchmark Optimization Analysis Complete ==========" -ForegroundColor Green
Write-Host ""
Write-Host "Total execution time: $([Math]::Round($Duration.TotalMinutes, 2)) minutes" -ForegroundColor Yellow
Write-Host "Results saved to: $ResultsDir" -ForegroundColor Yellow
Write-Host ""

# List generated files
Write-Host "Generated result files:" -ForegroundColor Cyan
Get-ChildItem "$ResultsDir\benchmark_*_O*.csv" -ErrorAction SilentlyContinue | ForEach-Object {
    $Size = $_.Length / 1KB
    Write-Host "  * $($_.Name) ($([Math]::Round($Size, 1)) KB)" -ForegroundColor Green
}
Write-Host ""

# Next steps
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Analyze: python analyze_optimization_levels.py results/benchmark_*_O*.csv" -ForegroundColor Cyan
Write-Host "  2. Compare optimization levels for performance impact" -ForegroundColor Cyan
Write-Host ""
