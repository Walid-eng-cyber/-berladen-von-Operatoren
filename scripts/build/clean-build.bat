@echo off
REM Clean build artifacts
echo Cleaning build directory...
if exist "build" (
    rmdir /s /q build
    echo Build directory removed.
) else (
    echo Build directory not found.
)
echo.
echo Clean complete!
pause
