#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "AppState.h"

class GDIPlusHelper {
public:
    static void InitGDIPlus(ULONG_PTR& gdiplusToken);
    static void ShutdownGDIPlus(ULONG_PTR gdiplusToken);
    static void RenderWindow(HDC hdc, const AppState& state, Gdiplus::Image* bgImage);
};
