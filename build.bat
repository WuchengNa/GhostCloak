@echo off
setlocal
cd /d "%~dp0"

echo [Build] Checking for MSVC...
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: MSVC compiler 'cl.exe' not found.
    echo Please run this script from 'Developer Command Prompt for VS'.
    pause
    exit /b 1
)

echo [Build] Compiling GhostCloak...
if not exist build mkdir build

cl /nologo /O2 /W3 /EHsc /D_UNICODE /DUNICODE /I"include" ^
    src\main.cpp ^
    src\ConfigManager.cpp ^
    src\BackgroundManager.cpp ^
    src\UIManager.cpp ^
    src\AutoClickManager.cpp ^
    src\GDIPlusHelper.cpp ^
    src\WindowHelper.cpp ^
    src\DragManager.cpp ^
    /Fe:build\GhostCloak.exe ^
    /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib gdiplus.lib shlwapi.lib shell32.lib

if %errorlevel% neq 0 (
    echo [Build] Failed!
    pause
    exit /b 1
)

echo [Build] Success! Output: build\GhostCloak.exe
echo.
pause
