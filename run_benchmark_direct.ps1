# Setup environment and run benchmarks

$env:PATH = "C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin;C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin;" + $env:PATH

cd "c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# Run original benchmarks with fast settings
Write-Host "Running original benchmarks..." -ForegroundColor Green
.\bin\matrix_google_benchmark.exe --benchmark_min_time=100ms --benchmark_out="results\benchmark_original_fast.csv" --benchmark_out_format=csv

Write-Host "Done!" -ForegroundColor Green
Get-ChildItem results\ -File | Format-Table Name, Length
