#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "AppState.h"

using namespace Gdiplus;

class BackgroundManager {
public:
    BackgroundManager(AppState& state);
    ~BackgroundManager();
    
    void GrabBackground(HWND hWnd);
    void RestoreBackground();
    void ResetBackground(HWND hWnd);
    Image* GetBackgroundImage() const;
    
private:
    AppState& m_state;
    Image* m_bgImage = NULL;
    
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};
