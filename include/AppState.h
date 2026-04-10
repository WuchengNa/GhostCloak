#pragma once
#include <string>
#include <vector>

// Per-click-point auto click configuration
struct AutoClickPoint {
    int x = 50;
    int y = 50;
    int size = 50;
    int intervalMs = 3000; // interval for this point in milliseconds
};

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
    bool autoClickSuspendedByMenu = false;
    bool lockWindowPosition = false;
    bool holeFollowCursor = false;
    int dotX = 50, dotY = 50;
    const int dotSize = 50;
    std::vector<AutoClickPoint> autoClickPoints;
    int activeAutoClickPoint = -1;
    int contextMenuX = 0;
    int contextMenuY = 0;
};
