# Windows Development Environment Setup Guide

Your Matrix class implementation is complete and ready to compile, but you need to install a C++ compiler first. Here are the recommended options:

## Option 1: MinGW-w64 (Recommended for Your Project)

This is the most straightforward option for Windows development.

### Step 1: Download MinGW-w64

1. Visit: https://www.mingw-w64.org/
2. Click "Compiler: GCC"
3. Under "Online Installers", download the latest version (e.g., `mingw-w64-install.exe`)

### Step 2: Install MinGW-w64

1. Run the installer
2. Choose settings:
   - **Version**: Latest (e.g., 14.1)
   - **Architecture**: x86_64
   - **Threads**: posix
   - **Exception Handling**: seh
   - **Build Revision**: Latest
3. **Installation Folder**: `C:\mingw64` (recommended)
4. Click "Install"

### Step 3: Add to System PATH

1. Press `Win + X` → "System"
2. Click "Advanced system settings" → "Environment Variables"
3. Under "System variables", find "Path" and click "Edit"
4. Click "New" and add: `C:\mingw64\bin`
5. Click OK on all dialogs
6. **Restart your terminal/PowerShell**

### Step 4: Verify Installation

Open a new PowerShell/Command Prompt and run:
```powershell
g++ --version
gcc --version
```

You should see version output.

## Option 2: LLVM/Clang (Alternative)

1. Visit: https://github.com/mstorsjo/llvm-mingw/releases
2. Download the latest `llvm-mingw-...-ucrt-x86_64.zip`
3. Extract to `C:\llvm-mingw`
4. Add `C:\llvm-mingw\bin` to your System PATH
5. Restart terminal and verify with: `clang++ --version`

## Option 3: Visual Studio Build Tools (For MSVC)

1. Download from: https://visualstudio.microsoft.com/downloads/
2. Select "Build Tools for Visual Studio 2022"
3. During installation, check "Desktop development with C++"
4. Complete installation

## After Installation: Building Your Project

### Using the batch script (simplest):

Update the compiler path in `build-direct.bat`:
- For MinGW: Change line 2 to: `set CLANG=g++`
- For LLVM: Change line 2 to: `set CLANG=clang++`

Then run:
```batch
.\build-direct.bat debug
.\build-direct.bat release
```

### Manual compilation with g++:

```powershell
# Create output directories
mkdir bin, obj

# Compile
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -DDEBUG_MATRIX -c code\matrix.cpp -o obj\matrix.obj
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -DDEBUG_MATRIX -Icode -c code\matrix_demo.cpp -o obj\matrix_demo.obj

# Link
g++ -std=c++17 obj\matrix.obj obj\matrix_demo.obj -o bin\matrix_demo.exe

# Run
.\bin\matrix_demo.exe
```

## Project Files

Your Matrix implementation includes:

- **matrix.h** - Matrix class header with constructor, destructor, copy constructor, and copy assignment
- **matrix.cpp** - Complete implementation with statistics tracking
- **matrix_demo.cpp** - Demonstration program showing all features
- **RVO.cpp** & **RVO_NO_OPTIMIZATION.cpp** - RVO optimization examples
- **CMakeLists.txt** - CMake build configuration (for use after CMake is installed)
- **Makefile** - GNU Make configuration (for use after Make is installed)

## Next Steps

1. **Install a C++ compiler** (MinGW-w64 recommended)
2. **Restart your terminal** to apply PATH changes
3. **Run build-direct.bat** or compile manually
4. **Execute the demo**: `.\bin\matrix_demo.exe`

## Troubleshooting

**"g++ not found":**
- Verify MinGW installed to `C:\mingw64`
- Check that `C:\mingw64\bin` is in your System PATH
- Restart PowerShell after adding to PATH

**"No such file or directory":**
- Ensure you're running from the project root directory
- Use forward slashes or escaped backslashes in paths

**Permission denied:**
- Run PowerShell as Administrator

## What's in the Matrix Class

The implementation includes:

✓ **Constructor** - Allocates memory, initializes to 0.0
✓ **Destructor** - Properly deallocates memory
✓ **Copy Constructor** - Creates deep copies
✓ **Copy Assignment** - Handles dimension changes and self-assignment
✓ **Element Access** - Via `at()` (with bounds checking) and `operator()`
✓ **Statistics** - Tracks instances, copies, and moves for benchmarking
✓ **Debug Output** - Optional debug printing with DEBUG_MATRIX flag
✓ **Print Method** - Nice matrix display
✓ **Fill Method** - Initialize all elements

## Example Usage

```cpp
Matrix A(3, 3);
A.fill(1.0);

// Copy constructor
Matrix B = A;

// Copy assignment
Matrix C(2, 2);
C = B;  // Automatically handles dimension mismatch

// Element access
double val = A(0, 0);
A(1, 2) = 5.5;

// Statistics
Matrix::printStats();
```

## Testing After Build

Run the demo to verify everything works:
```powershell
.\bin\matrix_demo.exe
```

You should see output showing:
- Matrix creation
- Copy operations
- Element access
- Statistics summary

---

**Questions?** Refer to this guide or check the README files in the project root.
