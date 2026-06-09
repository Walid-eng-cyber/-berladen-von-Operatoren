@echo off
REM Build script for Windows - Debug configuration
echo.
echo ========== Building Debug Configuration ==========
echo.

if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo Configuring CMake for Debug build...
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Debug

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========== Build Complete ==========
echo Executables are in: bin\ folder
echo.
pause
