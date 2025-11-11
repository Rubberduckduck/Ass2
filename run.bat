@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0"

:: Check if file was dragged onto the batch file
if not "%~1"=="" (
    set "IMAGE_PATH=%~1"
    echo Using dragged file: %IMAGE_PATH%
) else (
    set "IMAGE_PATH=input.jpg"
    echo Using default: %IMAGE_PATH%
)

:: Prompt for dimensions and method
set /p TARGET_WIDTH=Enter target width (default: 200): 
if "%TARGET_WIDTH%"=="" set "TARGET_WIDTH=200"

set /p TARGET_HEIGHT=Enter target height (default: 200): 
if "%TARGET_HEIGHT%"=="" set "TARGET_HEIGHT=200"

set /p METHOD=Enter method - dp/gp (default: dp): 
if "%METHOD%"=="" set "METHOD=dp"

echo.
echo Processing: %IMAGE_PATH%
echo Target size: %TARGET_WIDTH%x%TARGET_HEIGHT%
echo Method: %METHOD%
echo.

.\build\Release\seam_carving.exe "%IMAGE_PATH%" %TARGET_WIDTH% %TARGET_HEIGHT% %METHOD%

pause
endlocal
