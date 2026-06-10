# CMake Installation Guide for Windows

CMake is required to build this project. Follow one of the methods below to install it.

## Method 1: Download from Official Website (Recommended)

1. Visit https://cmake.org/download/
2. Download the Windows x64 ZIP file (Latest version)
3. Extract to a location like `C:\cmake` or `C:\Program Files\CMake`
4. **Add to System PATH:**
   - Press `Win + X`, search for "Environment Variables"
   - Click "Edit the system environment variables"
   - Click "Environment Variables..." button
   - In "System variables", find or create "Path"
   - Click "Edit..." and add the CMake bin directory (e.g., `C:\cmake\bin`)
   - Click OK and restart your terminal

5. Verify installation:
   ```powershell
   cmake --version
   ```

## Method 2: Using Chocolatey (If Installed)

If you have Chocolatey package manager installed:

```powershell
choco install cmake -y
```

Then verify:
```powershell
cmake --version
```

## Method 3: Using Windows Package Manager

If you have Windows Package Manager installed:

```powershell
winget install Kitware.CMake
```

Then verify:
```powershell
cmake --version
```

## Method 4: Visual Studio Build Tools

If you have Visual Studio installed, CMake may already be available. Try:

```powershell
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --version
```

## Post-Installation

After installing CMake, verify the installation worked:

```powershell
cmake --version
```

You should see output like:
```
cmake version 3.28.0

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

## Building After CMake Installation

Once CMake is installed, you can build the project:

### Using batch scripts:
```batch
build-debug.bat
```

Or build manually:
```powershell
cd "C:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
mkdir build -ErrorAction SilentlyContinue
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
cmake --build .
```

## Troubleshooting

**"cmake: The term 'cmake' is not recognized"**
- Restart your terminal/PowerShell after adding CMake to PATH
- Or add the full path: `C:\cmake\bin\cmake --version`

**"Generator 'Unix Makefiles' is not available"**
- You may need to install MinGW/LLVM (which you already have based on clang++)
- Or use a different generator like Visual Studio if installed

## Generator Options

To see available generators on your system:
```powershell
cmake --help
```

Some common generators:
- `"Unix Makefiles"` - Requires MinGW or LLVM
- `"Visual Studio 17 2022"` - Requires Visual Studio installed
- `"Ninja"` - Requires Ninja build tool
- `"NMake Makefiles"` - Requires NMake (VS tools)

## Next Steps

Once CMake is installed and working, you can:
1. Run `build-debug.bat` for debug build
2. Run `build-release.bat` for optimized release build
3. Executables will be in `build/bin/`
