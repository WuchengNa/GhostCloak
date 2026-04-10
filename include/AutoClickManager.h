#pragma once
#include <windows.h>
#include "AppState.h"

class AutoClickManager {
public:
    AutoClickManager(AppState& state);
    
    void StartTimer(HWND hWnd);
    void StopTimer(HWND hWnd);
    void PerformClick(HWND hWnd);
    void ResetTimer(HWND hWnd);
    
private:
    AppState& m_state;
    int m_currentPointIndex = 0;
    
    int GetCurrentPointInterval() const;
    void AdvanceToNextPoint();
};
