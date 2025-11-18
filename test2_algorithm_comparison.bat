@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0"

echo ============================================
echo    TEST 2: ALGORITHM COMPARISON
echo ============================================
echo.

:: Check if executable exists
if not exist ".\build\Release\seam_carving.exe" (
    echo ERROR: seam_carving.exe not found!
    echo Looking in: %cd%\build\Release\
    pause
    exit /b 1
)

:: Check if input file exists
if not exist "input.jpg" (
    echo ERROR: input.jpg not found!
    echo Looking in: %cd%
    pause
    exit /b 1
)

echo Found executable and input file
echo.

:: Set environment variable to disable GUI
set SEAM_CARVING_NO_DISPLAY=1
echo GUI disabled for automated testing
echo.

echo Algorithm Comparison - DP vs Greedy
echo ============================================
echo.
echo Testing both algorithms on 400x300 resize...
echo.
echo Algorithm    Elapsed Time    Output Status
echo --------------------------------------------------------

:: Test DP
call :get_centiseconds start_dp
.\build\Release\seam_carving.exe input.jpg 400 300 dp >nul 2>&1
call :get_centiseconds end_dp
set /a elapsed_dp=(end_dp - start_dp) * 10

if exist "output_dp.jpg" (
    set dp_status=SUCCESS
) else (
    set dp_status=FAILED
)
echo DP           !elapsed_dp! ms         !dp_status!

:: Clean up before greedy test
if exist "output_dp.jpg" del "output_dp.jpg" >nul
if exist "output_scaled.jpg" del "output_scaled.jpg" >nul

:: Test Greedy
call :get_centiseconds start_gp
.\build\Release\seam_carving.exe input.jpg 400 300 gp >nul 2>&1
call :get_centiseconds end_gp
set /a elapsed_gp=(end_gp - start_gp) * 10

if exist "output_gp.jpg" (
    set gp_status=SUCCESS
) else (
    set gp_status=FAILED
)
echo Greedy       !elapsed_gp! ms         !gp_status!

:: Calculate performance difference
if !elapsed_dp! gtr 0 (
    set /a speedup_percent=(elapsed_dp - elapsed_gp) * 100 / elapsed_dp
    echo.
    echo Result: Greedy is !speedup_percent!%% faster than DP
)

:: Clean up
if exist "output_gp.jpg" del "output_gp.jpg" >nul
if exist "output_scaled.jpg" del "output_scaled.jpg" >nul

echo.
echo Test 2 completed successfully!
echo.
pause
exit /b

:get_centiseconds
:: Convert current time to centiseconds (1/100 second)
:: %~1 = output variable name
for /f "tokens=1-4 delims=:.,- " %%a in ("%time: =0%") do (
    set /a "%~1=(((1%%a %% 100)*60+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)
exit /b
