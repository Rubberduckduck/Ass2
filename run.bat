@echo off
setlocal

:: Stay in current directory
cd /d "%~dp0"

:: Set path to your image and parameters
set "IMAGE_PATH=input.jpg"
set "TARGET_WIDTH=600"
set "TARGET_HEIGHT=300"
set "METHOD=dp"

:: Run the program using full path to executable
.\build\Release\seam_carving.exe "%IMAGE_PATH%" %TARGET_WIDTH% %TARGET_HEIGHT% %METHOD%

pause
endlocal