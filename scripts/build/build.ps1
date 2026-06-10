# Direct Build Script for Windows PowerShell
# Compiles the Matrix project directly without CMake or Make

param(
    [ValidateSet("debug", "release")]
    [string]$BuildType = "debug"
)

Write-Host "========== Operator Overloading Build System ==========" -ForegroundColor Green
Write-Host "Build Type: $BuildType`n" -ForegroundColor Cyan

# Configuration
$ClangPath = "C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin\clang++.exe"
$SourceDir = "code"
$BinDir = "bin"
$ObjDir = "obj"
$CxxStandard = "c++17"

# Compiler flags
$CommonFlags = @(
    "-std=$CxxStandard",
    "-Wall",
    "-Wextra",
    "-Wpedantic"
)

if ($BuildType -eq "debug") {
    $CommonFlags += @("-g", "-O0", "-DDEBUG_MATRIX")
}
else {
    $CommonFlags += @("-O2", "-DNDEBUG")
}

# Check if clang++ exists
if (-not (Test-Path $ClangPath)) {
    Write-Host "Error: clang++ not found at $ClangPath" -ForegroundColor Red
    Exit 1
}

Write-Host "Compiler: $ClangPath" -ForegroundColor Yellow
Write-Host "Flags: $($CommonFlags -join ' ')`n" -ForegroundColor Yellow

# Create directories
Write-Host "Creating directories..." -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path $BinDir | Out-Null
New-Item -ItemType Directory -Force -Path $ObjDir | Out-Null

# Compile matrix.cpp
Write-Host "Compiling matrix.cpp..." -ForegroundColor Cyan
$MatrixObj = "$ObjDir\matrix.obj"
& $ClangPath $CommonFlags -c "$SourceDir\matrix.cpp" -o $MatrixObj
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Compiled: $MatrixObj" -ForegroundColor Green

# Compile and link matrix_demo.exe
Write-Host "`nCompiling matrix_demo.cpp..." -ForegroundColor Cyan
$MatrixDemoObj = "$ObjDir\matrix_demo.obj"
& $ClangPath $CommonFlags -I"$SourceDir" -c "$SourceDir\matrix_demo.cpp" -o $MatrixDemoObj
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Compiled: $MatrixDemoObj" -ForegroundColor Green

Write-Host "Linking matrix_demo.exe..." -ForegroundColor Cyan
$MatrixDemoExe = "$BinDir\matrix_demo.exe"
& $ClangPath $CommonFlags $MatrixObj $MatrixDemoObj -o $MatrixDemoExe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Linking failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Linked: $MatrixDemoExe" -ForegroundColor Green

# Compile RVO.cpp
Write-Host "`nCompiling RVO.cpp..." -ForegroundColor Cyan
$RvoObj = "$ObjDir\RVO.obj"
& $ClangPath $CommonFlags -c "$SourceDir\RVO.cpp" -o $RvoObj
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Compiled: $RvoObj" -ForegroundColor Green

Write-Host "Linking rvo_example.exe..." -ForegroundColor Cyan
$RvoExe = "$BinDir\rvo_example.exe"
& $ClangPath $CommonFlags $RvoObj -o $RvoExe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Linking failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Linked: $RvoExe" -ForegroundColor Green

# Compile RVO_NO_OPTIMIZATION.cpp
Write-Host "`nCompiling RVO_NO_OPTIMIZATION.cpp..." -ForegroundColor Cyan
$RvoNoOptObj = "$ObjDir\RVO_NO_OPTIMIZATION.obj"
& $ClangPath $CommonFlags -c "$SourceDir\RVO_NO_OPTIMIZATION.cpp" -o $RvoNoOptObj
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Compiled: $RvoNoOptObj" -ForegroundColor Green

Write-Host "Linking rvo_no_optimization.exe..." -ForegroundColor Cyan
$RvoNoOptExe = "$BinDir\rvo_no_optimization.exe"
& $ClangPath $CommonFlags $RvoNoOptObj -o $RvoNoOptExe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Linking failed!" -ForegroundColor Red
    Exit 1
}
Write-Host "  ✓ Linked: $RvoNoOptExe" -ForegroundColor Green

Write-Host "`n========== Build Complete ==========" -ForegroundColor Green
Write-Host "Build Type: $BuildType" -ForegroundColor Cyan
Write-Host "Executables in: bin" -ForegroundColor Green
Write-Host 'To run: .\bin\matrix_demo.exe' -ForegroundColor Yellow
Write-Host ""
