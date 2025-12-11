#include "GDIPlusHelper.h"
#include <gdiplus.h>

void GDIPlusHelper::InitGDIPlus(ULONG_PTR& gdiplusToken) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void GDIPlusHelper::ShutdownGDIPlus(ULONG_PTR gdiplusToken) {
    GdiplusShutdown(gdiplusToken);
}

void GDIPlusHelper::RenderWindow(HDC hdc, const AppState& state, Image* bgImage) {
    RECT rc;
    GetClientRect(WindowFromDC(hdc), &rc);
    int w = rc.right;
    int h = rc.bottom;

    // Double Buffering
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBm = CreateCompatibleBitmap(hdc, w, h);
    HBITMAP hOldBm = (HBITMAP)SelectObject(hMemDC, hBm);

    Graphics graphics(hMemDC);

    // 1. Draw Background
    if (state.bgMode && bgImage) {
        graphics.DrawImage(bgImage, 0, 0, w, h);
    } else {
        SolidBrush bgBrush(Color(255, 51, 51, 51));
        graphics.FillRectangle(&bgBrush, 0, 0, w, h);
    }

    // 2. Draw Hole Frame (Gray)
    SolidBrush frameBrush(Color(255, 128, 128, 128));
    graphics.FillRectangle(&frameBrush, state.holeX, state.holeY, state.holeW, state.holeH);

    // 3. Draw Inner Hole (Green/Transparent)
    int b = 2;
    SolidBrush holeBrush(Color(255, 0, 255, 0));
    graphics.FillRectangle(&holeBrush, state.holeX + b, state.holeY + b, state.holeW - 2*b, state.holeH - 2*b);

    // 4. Draw Resize Handle for Hole (Bottom-Right)
    SolidBrush handleBrush(Color(255, 0, 0, 0));
    graphics.FillRectangle(&handleBrush, state.holeX + state.holeW - 15, state.holeY + state.holeH - 15, 15, 15);

    // 5. Draw AutoClick Dot (if enabled)
    if (state.autoClick) {
        SolidBrush dotBrush(Color(64, 0, 255, 0));
        graphics.FillEllipse(&dotBrush, state.dotX, state.dotY, state.dotSize, state.dotSize);
        
        Pen dotPen(Color(255, 0, 255, 0), 1);
        graphics.DrawEllipse(&dotPen, state.dotX, state.dotY, state.dotSize, state.dotSize);
    }

    // 6. Blit to Screen
    BitBlt(hdc, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, hOldBm);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
}
