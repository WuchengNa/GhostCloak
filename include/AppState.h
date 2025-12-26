#pragma once
#include <string>

// Global Application State Structure
struct AppState {
    // Window Properties
    int winX = -1, winY = -1, winW = 800, winH = 600;
    
    // Hole Properties
    int holeX = 100, holeY = 100, holeW = 300, holeH = 200;
    
    // Transparency
    BYTE alpha = 153; // 0.6 * 255
    bool bgMode = false;
    
    // File Paths
    std::wstring configPath;
    std::wstring cachePath;
    
    // Auto Clicker Properties
    bool autoClick = false;
    bool lockWindowPosition = false;
    bool holeFollowCursor = false;
    int dotX = 50, dotY = 50;
    const int dotSize =50;
};
