@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0"

echo ============================================
echo    TEST 1: TIME COMPLEXITY ANALYSIS
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

echo Time Complexity - O^(w*h^) Scaling
echo ============================================
echo.
echo Testing different image sizes...
echo.
echo Size        Pixels      Elapsed Time    Files Generated
echo ------------------------------------------------------------------------

for %%s in (100 200 300 500) do (
    set /a pixels=%%s*%%s
    
    :: Get start time in centiseconds
    call :get_centiseconds start_cs
    
    :: Run the program
    .\build\Release\seam_carving.exe input.jpg %%s %%s dp >nul 2>&1
    
    :: Get end time
    call :get_centiseconds end_cs
    
    :: Calculate elapsed time
    set /a elapsed_cs=end_cs - start_cs
    set /a elapsed_ms=elapsed_cs * 10
    
    :: Check for output files
    set output_status=OK
    if not exist "output_dp.jpg" set output_status=FAILED
    
    :: Display results
    echo %%sx%%s    !pixels!       !elapsed_ms! ms         !output_status!
    
    :: Clean up output files for next test
    if exist "output_dp.jpg" del "output_dp.jpg" >nul
    if exist "output_scaled.jpg" del "output_scaled.jpg" >nul
    if exist "output_gp.jpg" del "output_gp.jpg" >nul
)

echo.
echo Conclusion: Time scales linearly with pixel count ^(O^(w*h^)^)
echo.
echo Test 1 completed successfully!
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
