@echo off

set SCRIPT_DIR=%~dp0..\Script\
set SOLUTION_DIR=%~dp0..\..\Solution
set TEMP_DIR=%~dp0..\..\Temp

if not exist "%TEMP_DIR%" (
    mkdir "%TEMP_DIR%"
)

python "%SCRIPT_DIR%cli.py" build ^
    --solution-path "%SOLUTION_DIR%" ^
    --config "%1" ^
    --log-file-path "%TEMP_DIR%"

pause