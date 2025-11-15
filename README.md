
# Seam Carving Compressing - Build Guide

This project is maintained at [Rubberduckduck/AlgoAss2](https://github.com/Rubberduckduck/AlgoAss2).
It uses modern CMake and vcpkg for dependency management: **no manual OpenCV installation required!**

---

## Prerequisites

- **Windows 10/11**
- **Visual Studio 2022** (Community Edition works)
  - C++ Desktop Development workload
- **CMake >= 3.20** ([Download here](https://cmake.org/download/))
- **Git** ([Download here](https://git-scm.com/download/win))

---

## Getting Started

> **All commands below should be run in a Windows PowerShell window.**
> You can open PowerShell from the Start menu, or type `powershell` in the search bar.

### 1. Clone the Project

```powershell
git clone https://github.com/Rubberduckduck/AlgoAss2.git
cd AlgoAss2
```

### 2. Get vcpkg

If you haven't installed vcpkg yet:

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```

*(You only need to do this once per machine!)*

Return to the `AlgoAss2` folder when done.

### 3. Build the Project

Simply run the provided batch file:

```powershell
./build.bat
```
This will configure and build the project with CMake and vcpkg.

### 4. Run the Application

After building, start the program using:

```powershell
./run.bat
```
The application will prompt you for any required user input.

---

## Troubleshooting

- Make sure you always use the [x64 Native Tools Command Prompt for VS 2022](https://docs.microsoft.com/en-us/cpp/build/how-to-open-a-project-from-the-command-prompt?view=msvc-160) when building!
- If you experience `Could not find OpenCVConfig.cmake` errors, make sure you are using the latest `vcpkg` and have run the install step above.

---

## Developer Notes

- The build system uses `CMakeLists.txt`, so advanced users can also build manually:
  ```powershell
  cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
  cmake --build build --config Release
  ```
- If you add dependencies, update `vcpkg.json` and rerun `vcpkg install`.

---

## Support

If you hit issues building or running, check:

- The [vcpkg troubleshooting guide](https://learn.microsoft.com/vcpkg/troubleshoot/build-failures?WT.mc_id=vcpkg_inproduct_cli)
- Open an issue on [GitHub Issues](https://github.com/Rubberduckduck/AlgoAss2/issues)

---

**Enjoy Seam Carving Compressing!**
Feel free to reach out with questions or suggestions.
