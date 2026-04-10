#include "WindowHelper.h"
#include <windows.h>

void WindowHelper::InitDPI() {
    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (hUser32) {
        typedef BOOL(WINAPI* SetProcessDpiAwarenessContextProc)(DPI_AWARENESS_CONTEXT);
        auto setDpiCtx = (SetProcessDpiAwarenessContextProc)GetProcAddress(hUser32, "SetProcessDpiAwarenessContext");
        if (setDpiCtx) {
            setDpiCtx(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    
    HMODULE hShcore = LoadLibraryW(L"shcore.dll");
    if (hShcore) {
        typedef HRESULT(WINAPI* SetProcessDpiAwarenessProc)(int);
        auto setDpi = (SetProcessDpiAwarenessProc)GetProcAddress(hShcore, "SetProcessDpiAwareness");
        if (setDpi) setDpi(1);
        FreeLibrary(hShcore);
    }
}

bool WindowHelper::PtInRect(int x, int y, int rx, int ry, int rw, int rh) {
    return (x >= rx && x < rx + rw && y >= ry && y < ry + rh);
}

LRESULT WindowHelper::HandleNCHitTest(HWND hWnd, const AppState& state, POINT pt) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int m = 8;

    // Check AutoClick Points (Highest Priority)
    for (const auto& clickPoint : state.autoClickPoints) {
        if (PtInRect(pt.x, pt.y, clickPoint.x, clickPoint.y, clickPoint.size, clickPoint.size)) {
            return HTCLIENT;
        }
    }

    // Check if inside Hole
    if (PtInRect(pt.x, pt.y, state.holeX, state.holeY, state.holeW, state.holeH)) {
        return HTCLIENT;
    }

    // Check Window Borders
    if (pt.x < m && pt.y < m) return HTTOPLEFT;
    if (pt.x > rc.right - m && pt.y < m) return HTTOPRIGHT;
    if (pt.x < m && pt.y > rc.bottom - m) return HTBOTTOMLEFT;
    if (pt.x > rc.right - m && pt.y > rc.bottom - m) return HTBOTTOMRIGHT;
    if (pt.x < m) return HTLEFT;
    if (pt.x > rc.right - m) return HTRIGHT;
    if (pt.y < m) return HTTOP;
    if (pt.y > rc.bottom - m) return HTBOTTOM;

    if (state.lockWindowPosition) return HTCLIENT;
    return HTCAPTION;
} 

void WindowHelper::SetTransparency(HWND hWnd, BYTE alpha) {
    SetLayeredWindowAttributes(hWnd, RGB(0, 255, 0), alpha, LWA_COLORKEY | LWA_ALPHA);
}
