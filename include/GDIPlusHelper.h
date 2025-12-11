#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "AppState.h"

using namespace Gdiplus;

class GDIPlusHelper {
public:
    static void InitGDIPlus(ULONG_PTR& gdiplusToken);
    static void ShutdownGDIPlus(ULONG_PTR gdiplusToken);
    
    static void RenderWindow(HDC hdc, const AppState& state, Image* bgImage);
};
