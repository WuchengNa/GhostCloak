@echo off
REM GhostCloak CMake Build Script
setlocal enabledelayedexpansion

echo ================================
echo GhostCloak CMake Build System
echo ================================
echo.

REM Check if CMake is installed
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: CMake not found in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build

REM Navigate to build directory
cd build

REM Configure with CMake
echo [1/3] Configuring project with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo Error: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/3] Building project...
REM Build Release
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Error: Build failed!
    pause
    exit /b 1
)

echo.
echo [3/3] Build completed successfully!
echo.
echo Output executable: bin\Release\GhostCloak.exe
echo.
pause

cd ..
