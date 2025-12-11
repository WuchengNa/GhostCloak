#!/usr/bin/env pwsh
<#
.SYNOPSIS
    GhostCloak CMake Build Script
.DESCRIPTION
    Automates the CMake configuration and build process for GhostCloak
.EXAMPLE
    .\cmake_build.ps1
    .\cmake_build.ps1 -BuildType Debug
    .\cmake_build.ps1 -Clean
#>

param(
    [Parameter(Mandatory=$false)]
    [ValidateSet('Debug', 'Release')]
    [string]$BuildType = 'Release',
    
    [Parameter(Mandatory=$false)]
    [switch]$Clean,
    
    [Parameter(Mandatory=$false)]
    [switch]$Help
)

function Show-Help {
    Write-Host @"
GhostCloak CMake Build Script

Usage: .\cmake_build.ps1 [options]

Options:
  -BuildType   Build type: Debug or Release (default: Release)
  -Clean       Clean build directory before building
  -Help        Show this help message

Examples:
  .\cmake_build.ps1
  .\cmake_build.ps1 -BuildType Debug
  .\cmake_build.ps1 -Clean -BuildType Release
"@
}

if ($Help) {
    Show-Help
    exit 0
}

# Check if CMake is installed
try {
    $cmake = Get-Command cmake -ErrorAction Stop
    Write-Host "✓ CMake found: $($cmake.Source)" -ForegroundColor Green
} catch {
    Write-Host "✗ Error: CMake not found in PATH" -ForegroundColor Red
    Write-Host "Please install CMake from https://cmake.org/download/" -ForegroundColor Yellow
    exit 1
}

# Check if project root exists
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Host "✗ Error: CMakeLists.txt not found in current directory" -ForegroundColor Red
    exit 1
}

# Clean build directory if requested
if ($Clean -and (Test-Path "build")) {
    Write-Host "Cleaning build directory..." -ForegroundColor Cyan
    Remove-Item -Recurse -Force "build"
}

# Create build directory
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Configure
Write-Host "`n[1/3] Configuring project with CMake..." -ForegroundColor Cyan
Push-Location build
try {
    cmake .. -G "Visual Studio 17 2022" -A x64
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ CMake configuration failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Configuration successful" -ForegroundColor Green
} finally {
    Pop-Location
}

# Build
Write-Host "`n[2/3] Building project ($BuildType)..." -ForegroundColor Cyan
Push-Location build
try {
    cmake --build . --config $BuildType
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Build failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Build successful" -ForegroundColor Green
} finally {
    Pop-Location
}

# Show output
$outputPath = "build\bin\$BuildType\GhostCloak.exe"
Write-Host "`n[3/3] Build completed!" -ForegroundColor Green
Write-Host "`nOutput: $outputPath" -ForegroundColor Cyan

if (Test-Path $outputPath) {
    Write-Host "✓ Executable found" -ForegroundColor Green
} else {
    Write-Host "✗ Warning: Executable not found at expected location" -ForegroundColor Yellow
}

Write-Host ""
