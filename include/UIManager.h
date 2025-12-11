#pragma once
#include <windows.h>
#include <string>
#include "AppState.h"

class UIManager {
public:
    UIManager(AppState& state);
    
    void ShowContextMenu(HWND hWnd, POINT pt);
    
private:
    AppState& m_state;
};
