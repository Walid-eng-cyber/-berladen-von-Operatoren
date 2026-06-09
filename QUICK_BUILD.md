# Quick Build Reference

## TL;DR - After Installing a Compiler

### Option 1: Using the Batch Script (Easiest)

```batch
.\build-direct.bat debug    # Debug build with symbols
.\build-direct.bat release  # Optimized release build

# Run the demo
.\bin\matrix_demo.exe
```

### Option 2: Manual Compilation with g++

```powershell
# Navigate to project directory
cd "C:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"

# Create directories
mkdir bin, obj

# Compile matrix.cpp
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -DDEBUG_MATRIX -c code\matrix.cpp -o obj\matrix.obj

# Compile matrix_demo.cpp
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -DDEBUG_MATRIX -Icode -c code\matrix_demo.cpp -o obj\matrix_demo.obj

# Link everything
g++ -std=c++17 obj\matrix.obj obj\matrix_demo.obj -o bin\matrix_demo.exe

# Run
.\bin\matrix_demo.exe
```

### Option 3: Using CMake (After Installing CMake)

```powershell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Run from build\bin
.\bin\matrix_demo.exe
```

## Compiler Installation

Choose **ONE** option:

### MinGW-w64 (Recommended for Windows)
1. Download: https://www.mingw-w64.org/
2. Install to `C:\mingw64`
3. Add `C:\mingw64\bin` to System PATH
4. Restart terminal

### LLVM/Clang
1. Download: https://github.com/mstorsjo/llvm-mingw/releases
2. Extract to `C:\llvm-mingw`
3. Add `C:\llvm-mingw\bin` to System PATH
4. Restart terminal

### Verify Installation
```powershell
g++ --version      # For MinGW
# or
clang++ --version  # For LLVM
```

## Files to Build

### Core Files
- `code/matrix.cpp` - Matrix implementation
- `code/matrix_demo.cpp` - Demo program

### Optional Files
- `code/RVO.cpp` - RVO optimization example
- `code/RVO_NO_OPTIMIZATION.cpp` - Non-optimized example

## Build Configurations

### Debug Flags
```
-std=c++17 -Wall -Wextra -Wpedantic -g -O0 -DDEBUG_MATRIX
```
- Full debug symbols (`-g`)
- No optimization (`-O0`)
- Debug output enabled (`-DDEBUG_MATRIX`)
- Runtime checks enabled

### Release Flags
```
-std=c++17 -Wall -Wextra -Wpedantic -O2 -DNDEBUG
```
- Optimization level 2 (`-O2`)
- Smaller binary
- No debug output (`-DNDEBUG`)
- Assertions disabled

## Output Files

After building, find executables in:
```
.\bin\matrix_demo.exe              # Matrix demonstration
.\bin\rvo_example.exe              # RVO optimization demo
.\bin\rvo_no_optimization.exe      # Non-optimized example
```

## Troubleshooting

| Problem | Solution |
|---------|----------|
| "g++ not found" | Install MinGW-w64, add to PATH, restart terminal |
| "No such file or directory" | Run from project root, verify paths |
| "Permission denied" | Run PowerShell as Administrator |
| Build fails | Check all source files exist in `code/` folder |

## Next Steps After Building

1. Run matrix demo to test basic functionality
2. Review output to understand copy semantics
3. Implement move semantics for next phase
4. Add operator overloads
5. Run performance benchmarks

---

**Still need help?** See [SETUP_COMPILER.md](SETUP_COMPILER.md) for detailed installation guide.
