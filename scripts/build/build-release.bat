@echo off
REM Build script for Windows - Release configuration
echo.
echo ========== Building Release Configuration ==========
echo.

if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo Configuring CMake for Release build...
cmake .. -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

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
