@echo off
setlocal

:: Stay in current directory
cd /d "%~dp0"

:: Set path to your image and parameters
set "IMAGE_PATH=input.jpg"
set "TARGET_WIDTH=200"
set "TARGET_HEIGHT=200"
:: Check if method argument is provided, else default to dp
if "%1"=="" (
    set "METHOD=dp"
) else (
    set "METHOD=%1"
)

:: Run the program using full path to executable
.\build\Release\seam_carving.exe "%IMAGE_PATH%" %TARGET_WIDTH% %TARGET_HEIGHT% %METHOD%

pause
endlocal