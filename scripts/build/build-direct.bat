@echo off
setlocal enabledelayedexpansion

set CLANG=C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin\clang++.exe
set BUILDTYPE=debug
set SRCDIR=code
set BINDIR=bin
set OBJDIR=obj

if "%1"=="release" set BUILDTYPE=release

echo.
echo ========== Operator Overloading Build System ==========
echo Build Type: %BUILDTYPE%
echo.

if not exist "%BINDIR%" mkdir "%BINDIR%"
if not exist "%OBJDIR%" mkdir "%OBJDIR%"

set CXXFLAGS=-std=c++17 -Wall -Wextra -Wpedantic
if "%BUILDTYPE%"=="debug" (
    set CXXFLAGS=%CXXFLAGS% -g -O0 -DDEBUG_MATRIX
) else (
    set CXXFLAGS=%CXXFLAGS% -O2 -DNDEBUG
)

echo Compiling matrix.cpp...
%CLANG% %CXXFLAGS% -c "%SRCDIR%\matrix.cpp" -o "%OBJDIR%\matrix.obj"
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)
echo  OK: matrix.obj

echo Compiling matrix_demo.cpp...
%CLANG% %CXXFLAGS% -I"%SRCDIR%" -c "%SRCDIR%\matrix_demo.cpp" -o "%OBJDIR%\matrix_demo.obj"
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)
echo  OK: matrix_demo.obj

echo Linking matrix_demo.exe...
%CLANG% %CXXFLAGS% "%OBJDIR%\matrix.obj" "%OBJDIR%\matrix_demo.obj" -o "%BINDIR%\matrix_demo.exe"
if errorlevel 1 (
    echo Linking failed!
    exit /b 1
)
echo  OK: matrix_demo.exe

echo Compiling RVO.cpp...
%CLANG% %CXXFLAGS% -c "%SRCDIR%\RVO.cpp" -o "%OBJDIR%\RVO.obj"
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)
echo  OK: RVO.obj

echo Linking rvo_example.exe...
%CLANG% %CXXFLAGS% "%OBJDIR%\RVO.obj" -o "%BINDIR%\rvo_example.exe"
if errorlevel 1 (
    echo Linking failed!
    exit /b 1
)
echo  OK: rvo_example.exe

echo Compiling RVO_NO_OPTIMIZATION.cpp...
%CLANG% %CXXFLAGS% -c "%SRCDIR%\RVO_NO_OPTIMIZATION.cpp" -o "%OBJDIR%\RVO_NO_OPTIMIZATION.obj"
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)
echo  OK: RVO_NO_OPTIMIZATION.obj

echo Linking rvo_no_optimization.exe...
%CLANG% %CXXFLAGS% "%OBJDIR%\RVO_NO_OPTIMIZATION.obj" -o "%BINDIR%\rvo_no_optimization.exe"
if errorlevel 1 (
    echo Linking failed!
    exit /b 1
)
echo  OK: rvo_no_optimization.exe

echo.
echo ========== Build Complete ==========
echo Build Type: %BUILDTYPE%
echo Executables in: %BINDIR%
echo.
echo To run matrix demo:
echo   %BINDIR%\matrix_demo.exe
echo.
