#!/usr/bin/env powershell

<#
.SYNOPSIS
    Run All Matrix Benchmarks and Export CSV Results
    
.DESCRIPTION
    Runs both original and advanced benchmark suites,
    exports results to CSV format for analysis in Excel/Python.
    
.EXAMPLE
    .\run_all_benchmarks.ps1
    
.NOTES
    Results saved to: results/benchmark_results_<timestamp>.csv
#>

param(
    [ValidateSet('fast', 'normal', 'thorough')]
    [string]$Speed = 'normal'
)

# ============================================================
# Configuration
# ============================================================

$ProjectDir = "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
$BinDir = "$ProjectDir\bin"
$ResultsDir = "$ProjectDir\results"
$Timestamp = Get-Date -Format "yyyy-MM-dd_HHmmss"

# Benchmark time settings
$BenchmarkTime = @{
    'fast'     = '50ms'
    'normal'   = '500ms'
    'thorough' = '2s'
}[$Speed]

# ============================================================
# Setup
# ============================================================

Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║       Matrix Benchmarks - CSV Export Script               ║" -ForegroundColor Cyan
Write-Host "║       Mode: $Speed (Time: $BenchmarkTime)" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

# Set environment
$env:PATH = "C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin;C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH

# Create results directory
if (-not (Test-Path $ResultsDir)) {
    mkdir $ResultsDir | Out-Null
    Write-Host "✓ Created results directory" -ForegroundColor Green
}

cd $ProjectDir

# ============================================================
# Helper Functions
# ============================================================

function Invoke-Benchmark {
    param(
        [string]$Name,
        [string]$Executable,
        [string]$Filter,
        [string]$OutputFile
    )
    
    Write-Host ""
    Write-Host "Running: $Name" -ForegroundColor Yellow
    Write-Host "Time per benchmark: $BenchmarkTime" -ForegroundColor Gray
    
    if (-not (Test-Path $Executable)) {
        Write-Host "✗ Executable not found: $Executable" -ForegroundColor Red
        return $false
    }
    
    # Run benchmark and export to CSV
    $cmd = "& `"$Executable`" --benchmark_filter=$Filter --benchmark_out=`"$OutputFile`" --benchmark_out_format=csv --benchmark_min_time=$BenchmarkTime"
    
    Invoke-Expression $cmd 2>&1 | Out-Null
    
    if ($LASTEXITCODE -eq 0 -and (Test-Path $OutputFile)) {
        $size = (Get-Item $OutputFile).Length / 1KB
        Write-Host "✓ Exported: $OutputFile ($([math]::Round($size, 2)) KB)" -ForegroundColor Green
        return $true
    } else {
        Write-Host "✗ Export failed" -ForegroundColor Red
        return $false
    }
}

# ============================================================
# Run Benchmarks
# ============================================================

Write-Host ""
Write-Host "Starting benchmark runs..." -ForegroundColor Yellow

$results = @()

# Run original Google Benchmark suite
$originalOutput = "$ResultsDir\benchmark_original_$Timestamp.csv"
$success1 = Invoke-Benchmark -Name "Original Suite (15 benchmarks)" `
    -Executable "$BinDir\matrix_google_benchmark.exe" `
    -Filter "" `
    -OutputFile $originalOutput

if ($success1) { $results += $originalOutput }

# Run advanced benchmark suite
$advancedOutput = "$ResultsDir\benchmark_advanced_$Timestamp.csv"
$success2 = Invoke-Benchmark -Name "Advanced Suite (33 benchmarks)" `
    -Executable "$BinDir\matrix_benchmark_advanced.exe" `
    -Filter "" `
    -OutputFile $advancedOutput

if ($success2) { $results += $advancedOutput }

# ============================================================
# Combine Results
# ============================================================

Write-Host ""
Write-Host "Processing results..." -ForegroundColor Yellow

$combinedOutput = "$ResultsDir\benchmark_combined_$Timestamp.csv"
$mergedData = @()

foreach ($file in $results) {
    if (Test-Path $file) {
        $csv = Import-Csv $file
        $mergedData += $csv
    }
}

# Export combined results
if ($mergedData.Count -gt 0) {
    $mergedData | Export-Csv -Path $combinedOutput -NoTypeInformation -Encoding UTF8
    Write-Host "✓ Combined results: $combinedOutput" -ForegroundColor Green
}

# ============================================================
# Summary Report
# ============================================================

Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║              Benchmark Export Complete! ✓                  ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Green

Write-Host ""
Write-Host "Results Summary:" -ForegroundColor Cyan

if ($mergedData.Count -gt 0) {
    Write-Host "  Total benchmarks: $($mergedData.Count)" -ForegroundColor White
    
    # Extract performance metrics
    $times = @()
    foreach ($row in $mergedData) {
        if ($row.Time -match '(\d+(?:\.\d+)?)\s*([a-z]+)') {
            $times += $row.Time
        }
    }
    
    Write-Host "  Benchmark time range: $(($times | Measure-Object -Minimum).Minimum) - $(($times | Measure-Object -Maximum).Maximum)" -ForegroundColor White
}

Write-Host ""
Write-Host "Output Files:" -ForegroundColor Cyan
Write-Host "  Combined:  $combinedOutput" -ForegroundColor White

if ($success1) {
    Write-Host "  Original:  $originalOutput" -ForegroundColor White
}

if ($success2) {
    Write-Host "  Advanced:  $advancedOutput" -ForegroundColor White
}

Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Cyan
Write-Host "  1. Open in Excel: $combinedOutput" -ForegroundColor Gray
Write-Host "  2. Or process with Python:" -ForegroundColor Gray
Write-Host "     python analyze_benchmarks.py $combinedOutput" -ForegroundColor Gray
Write-Host ""

# ============================================================
# Generate Analysis Report
# ============================================================

Write-Host "Generating analysis summary..." -ForegroundColor Yellow

# Group benchmarks by category
$analysisFile = "$ResultsDir\analysis_$Timestamp.txt"
$analysis = @()
$analysis += "Matrix Benchmark Analysis Report"
$analysis += "Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$analysis += "========================================`n"

# V2 Performance
$analysis += "V2 (Move Semantics) - Copy vs Move:"
$copyBench = $mergedData | Where-Object { $_.name -like '*Copy_Constructor*' }
$moveBench = $mergedData | Where-Object { $_.name -like '*Move_Constructor*' }

if ($copyBench -and $moveBench) {
    $analysis += "  Copy Constructor: $($copyBench.time)"
    $analysis += "  Move Constructor: $($moveBench.time)"
    $analysis += ""
}

# V3 Performance
$analysis += "V3 (Compound Operators) - Free vs In-Place:"
$freeBench = $mergedData | Where-Object { $_.name -like '*FreeFunc*Free*' }
$inplaceBench = $mergedData | Where-Object { $_.name -like '*InPlace*' }

if ($freeBench -and $inplaceBench) {
    $analysis += "  Free Functions: $($freeBench.time)"
    $analysis += "  In-Place Operators: $($inplaceBench.time)"
    $analysis += ""
}

# Scaling
$analysis += "Scaling Analysis (Addition Operation):"
$scaling = $mergedData | Where-Object { $_.name -like '*Addition_Scaling*' }
$analysis += "  Benchmarks: $($scaling.Count)"
$analysis += ""

# Write analysis
$analysis | Out-File $analysisFile -Encoding UTF8
Write-Host "✓ Analysis report: $analysisFile" -ForegroundColor Green

Write-Host ""
Write-Host "✓ All benchmarks completed successfully!" -ForegroundColor Green
Write-Host ""
