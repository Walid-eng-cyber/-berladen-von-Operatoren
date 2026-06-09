# CMake Build System

This project uses CMake for cross-platform building with support for Debug and Release configurations.

## Requirements

- CMake 3.20 or later
- C++17 compatible compiler (GCC, Clang, or MSVC)
- On Windows: MinGW, LLVM, or Visual Studio with C++ tools

## Quick Start

### Using Build Scripts (Windows)

**Release Build:**
```batch
build-release.bat
```

**Debug Build:**
```batch
build-debug.bat
```

**Clean Build Artifacts:**
```batch
clean-build.bat
```

### Manual CMake Commands

**Create build directory and configure:**
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

**Build the project:**
```bash
cmake --build .
```

**Or for parallel builds:**
```bash
cmake --build . --parallel 4
```

## Build Configurations

### Release
- Optimization: `-O2` (GCC/Clang) or `/O2` (MSVC)
- Debug symbols: Disabled
- Definition: `NDEBUG` is set
- Best for: Performance analysis, benchmarking

### Debug
- Optimization: `-O0` (GCC/Clang) or `/Od` (MSVC)
- Debug symbols: Enabled (`-g` or `/Zi`)
- Runtime checks: Enabled
- Best for: Development, debugging

## Project Structure

```
-berladen-von-Operatoren/
├── CMakeLists.txt              # Root CMake configuration
├── code/
│   ├── CMakeLists.txt          # Code subdirectory configuration
│   ├── RVO.cpp                 # RVO optimization example
│   └── RVO_NO_OPTIMIZATION.cpp # Non-optimized example
├── build/                      # Build directory (created automatically)
│   └── bin/                    # Compiled executables
├── build-release.bat           # Release build script
├── build-debug.bat             # Debug build script
└── clean-build.bat             # Clean build artifacts
```

## Generated Executables

After building, executables are located in `build/bin/`:
- `rvo_example.exe` - RVO optimization demonstration
- `rvo_no_optimization.exe` - Non-optimized version

## Running Executables

### Release Build
```bash
cd build
./bin/rvo_example
./bin/rvo_no_optimization
```

### Debug Build
```bash
cd build
./bin/rvo_example
./bin/rvo_no_optimization
```

## Compiler Flags

### GCC/Clang Flags

**Debug:**
- `-Wall -Wextra` - Warning levels
- `-g` - Debug symbols
- `-O0` - No optimization

**Release:**
- `-Wall -Wextra` - Warning levels
- `-O2` - Optimization level 2
- `-DNDEBUG` - Disable assertions

### MSVC Flags

**Debug:**
- `/W4` - Warning level 4
- `/Zi` - Debug information
- `/Od` - Disable optimization
- `/RTC1` - Runtime checks

**Release:**
- `/W4` - Warning level 4
- `/O2` - Optimization level 2
- `/DNDEBUG` - Disable assertions

## Advanced Usage

### Build with Specific Generator

For Visual Studio:
```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release
```

For Ninja:
```bash
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

### Verbose Build

To see full compiler commands:
```bash
cmake --build . --verbose
```

### Install Targets (if configured)

```bash
cmake --install . --prefix <install_path>
```

## Troubleshooting

**CMake not found:**
- Install CMake from https://cmake.org/download/

**Compiler not found:**
- Ensure you have a C++ compiler installed (GCC, Clang, or MSVC)
- On Windows with clang++, ensure LLVM is properly installed

**Build fails with "Unknown generator":**
- Check available generators: `cmake --help`
- Use `-G` flag to specify the generator

**Permission denied (Linux/macOS):**
```bash
chmod +x build-*.sh
./build-release.sh
```

## Customization

To add new source files:

1. Update `code/CMakeLists.txt`
2. Add new executable or library target:
```cmake
add_executable(new_program new_file.cpp)
target_compile_features(new_program PRIVATE cxx_std_17)
```

## C++ Standard

This project requires **C++17** or later. To change the standard, edit the root `CMakeLists.txt`:
```cmake
set(CMAKE_CXX_STANDARD 20)  # For C++20
```

## Integration with VS Code

The CMake Tools extension provides convenient build and debug buttons in VS Code. After installing the extension, you can:
1. Select kit (compiler)
2. Select build variant (Debug/Release)
3. Build with the build button
4. Debug with F5 or the debug button

## Documentation

For more information on CMake, visit: https://cmake.org/documentation/
