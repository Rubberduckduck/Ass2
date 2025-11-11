@echo off
setlocal

:: Move to script directory
cd /d "%~dp0"

:: Check cmake exists
where cmake >nul 2>&1
if %errorlevel% neq 0 (
  echo.
  echo ================================
  echo ERROR: cmake not found in PATH.
  echo Install CMake or add it to PATH.
  echo ================================
  echo.
  pause
  exit /b 1
)

:: Configurable args: [config] [generator]
set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Release"
set "GEN=%~2"

:: Build directory
set "BUILD_DIR=%~dp0build"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

pushd "%BUILD_DIR%"

echo Configuring project (Config=%CONFIG% Generator=%GEN%)...
if "%GEN%"=="" (
  cmake -DCMAKE_BUILD_TYPE=%CONFIG% ..
) else (
  cmake -G "%GEN%" -DCMAKE_BUILD_TYPE=%CONFIG% ..
)
if %errorlevel% neq 0 (
  echo.
  echo ================================
  echo       BUILD FAILED
  echo CMake configuration failed.
  echo ================================
  echo.
  popd
  pause
  exit /b 1
)

echo Building...
cmake --build . --config %CONFIG% -- /m
if %errorlevel% neq 0 (
  echo.
  echo ================================
  echo       BUILD FAILED
  echo Build process failed.
  echo ================================
  echo.
  popd
  pause
  exit /b 1
)

echo.
echo ================================
echo     BUILD SUCCESSFUL!
echo Binaries in: %BUILD_DIR%
echo ================================
echo.
popd
pause
endlocal
exit /b 0
