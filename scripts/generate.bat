@echo off
setlocal

echo Generating project files using Premake...

REM Change to the project root directory (where premake5.lua lives)
cd ..

echo Using Premake script at: %cd%\premake5.lua

REM Run premake.exe from the tools directory
tools\premake5.exe vs2022

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Premake failed with exit code %ERRORLEVEL%.
    pause
    exit /b %ERRORLEVEL%
)

echo Project files generated successfully.
pause