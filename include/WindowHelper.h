#pragma once
#include <windows.h>
#include "AppState.h"

class WindowHelper {
public:
    static void InitDPI();
    static bool PtInRect(int x, int y, int rx, int ry, int rw, int rh);
    static LRESULT HandleNCHitTest(HWND hWnd, const AppState& state, POINT pt);
    static void SetTransparency(HWND hWnd, BYTE alpha);
};
