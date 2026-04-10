#include "GDIPlusHelper.h"
#include <gdiplus.h>

void GDIPlusHelper::InitGDIPlus(ULONG_PTR& gdiplusToken) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void GDIPlusHelper::ShutdownGDIPlus(ULONG_PTR gdiplusToken) {
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void GDIPlusHelper::RenderWindow(HDC hdc, const AppState& state, Gdiplus::Image* bgImage) {
    RECT rc;
    GetClientRect(WindowFromDC(hdc), &rc);
    int w = rc.right;
    int h = rc.bottom;

    // Double Buffering
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBm = CreateCompatibleBitmap(hdc, w, h);
    HBITMAP hOldBm = (HBITMAP)SelectObject(hMemDC, hBm);

    Gdiplus::Graphics graphics(hMemDC);

    // 1. Draw Background
    if (state.bgMode && bgImage) {
        graphics.DrawImage(bgImage, 0, 0, w, h);
    } else {
        Gdiplus::SolidBrush bgBrush(Gdiplus::Color(255, 51, 51, 51));
        graphics.FillRectangle(&bgBrush, 0, 0, w, h);
    }

    // 2. Draw Hole Frame (Gray)
    Gdiplus::SolidBrush frameBrush(Gdiplus::Color(255, 128, 128, 128));
    graphics.FillRectangle(&frameBrush, state.holeX, state.holeY, state.holeW, state.holeH);

    // 3. Draw Inner Hole (Green/Transparent)
    int b = 5; //border thickness
    Gdiplus::SolidBrush holeBrush(Gdiplus::Color(255, 0, 255, 0));
    graphics.FillRectangle(&holeBrush, state.holeX + b, state.holeY + b, state.holeW - 2 * b, state.holeH - 2 * b);

    // 4. Draw Resize Handle for Hole (Bottom-Right)
    Gdiplus::SolidBrush handleBrush(Gdiplus::Color(255, 0, 0, 0));
    graphics.FillRectangle(&handleBrush, state.holeX + state.holeW - 15, state.holeY + state.holeH - 15, 15, 15);

    // 5. Draw AutoClick Points if any are configured
    if (!state.autoClickPoints.empty()) {
        Gdiplus::SolidBrush dotBrush(Gdiplus::Color(64, 0, 255, 0));
        Gdiplus::Pen dotPen(Gdiplus::Color(255, 0, 255, 0), 1);
        for (const auto& point : state.autoClickPoints) {
            graphics.FillEllipse(&dotBrush, point.x, point.y, point.size, point.size);
            graphics.DrawEllipse(&dotPen, point.x, point.y, point.size, point.size);
        }
    }

    // 6. Blit to Screen
    BitBlt(hdc, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBm);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
}