@echo off
setlocal

:: Move to script directory
cd /d "%~dp0"

:: Remove build directory
if exist "build" (
    echo Removing build directory...
    rd /s /q "build"
)

:: Remove CMake cache files
if exist "CMakeCache.txt" del "CMakeCache.txt"
if exist "CMakeFiles" rd /s /q "CMakeFiles"

:: Remove Visual Studio files
if exist ".vs" rd /s /q ".vs"
if exist "*.sln" del "*.sln"
if exist "*.vcxproj*" del "*.vcxproj*"

echo Cleanup complete.
endlocal