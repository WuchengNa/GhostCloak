#include <windows.h>
#include <windowsx.h>
#include <string>
#include <ctime>
#include "AppState.h"
#include "ConfigManager.h"
#include "BackgroundManager.h"
#include "UIManager.h"
#include "AutoClickManager.h"
#include "GDIPlusHelper.h"
#include "WindowHelper.h"
#include "DragManager.h"

// Link with necessary libraries
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Shlwapi.lib")
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "Shell32.lib")

// Constants
#define ID_TRAY_EXIT        1001
#define ID_TRAY_GRAB        1002
#define ID_TRAY_RESET       1003
#define ID_TRAY_AUTOCLICK   1004
#define ID_TRAY_LOCKMOVE    1005
#define ID_TRAY_HOLEFOLLOW  1006
#define IDT_AUTOCLICK       999

const COLORREF TRANSPARENT_KEY = RGB(0, 255, 0);

// Global objects
AppState g_State;
HWND g_hWnd = NULL;
ULONG_PTR g_gdiplusToken;
ConfigManager* g_configMgr = NULL;
BackgroundManager* g_bgMgr = NULL;
UIManager* g_uiMgr = NULL;
AutoClickManager* g_autoClickMgr = NULL;
DragManager* g_dragMgr = NULL;

// Forward Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ==========================================
// Main Entry
// ==========================================
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    srand((unsigned)time(NULL));
    
    // 1. DPI Awareness
    WindowHelper::InitDPI();

    // 2. GDI+ Startup
    GDIPlusHelper::InitGDIPlus(g_gdiplusToken);

    // 3. Paths
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    g_State.configPath = std::wstring(tempPath) + L"ghost_config.ini";
    g_State.cachePath = std::wstring(tempPath) + L"ghost_bg_cache.png";

    // 4. Initialize managers
    g_configMgr = new ConfigManager(g_State);
    g_configMgr->LoadConfig();

    g_bgMgr = new BackgroundManager(g_State);
    g_uiMgr = new UIManager(g_State);
    g_autoClickMgr = new AutoClickManager(g_State);
    g_dragMgr = new DragManager();

    // 5. Create Window Class
    const wchar_t CLASS_NAME[] = L"GhostCloakClass";
    WNDCLASSW wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    RegisterClassW(&wc);

    // 6. Calculate Position (Center if first run)
    if (g_State.winX == -1) {
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        g_State.winX = (sw - g_State.winW) / 2;
        g_State.winY = (sh - g_State.winH) / 2;
    }

    // 7. Create Window (Layered + Popup)
    g_hWnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"GhostCloak",
        WS_POPUP,
        g_State.winX, g_State.winY, g_State.winW, g_State.winH,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hWnd) return 0;

    // 8. Set Transparency
    WindowHelper::SetTransparency(g_hWnd, g_State.alpha);

    // 9. Restore Background if needed
    if (g_State.bgMode) {
        g_bgMgr->RestoreBackground();
    }

    ShowWindow(g_hWnd, nCmdShow);

    // 10. Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    delete g_dragMgr;
    delete g_autoClickMgr;
    delete g_uiMgr;
    delete g_bgMgr;
    delete g_configMgr;
    GDIPlusHelper::ShutdownGDIPlus(g_gdiplusToken);
    return 0;
}

// ==========================================
// Window Procedure
// ==========================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Intercept ESC early: if we are in Hole-Follow mode, Esc should exit that mode and not close the app.
    if ((message == WM_KEYDOWN || message == WM_CHAR) && wParam == VK_ESCAPE) {
        if (g_State.holeFollowCursor) {
            g_State.holeFollowCursor = false;
            InvalidateRect(hWnd, NULL, FALSE);
            return 0;
        }
    }

    switch (message) {
    case WM_ENTERSIZEMOVE:
        if (g_State.autoClick) {
            g_autoClickMgr->StopTimer(hWnd);
        }
        return 0;

    case WM_EXITSIZEMOVE:
        g_autoClickMgr->StartTimer(hWnd);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        GDIPlusHelper::RenderWindow(hdc, g_State, g_bgMgr->GetBackgroundImage());
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_SETCURSOR: {
        if (LOWORD(lParam) == HTCLIENT) {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);

            if (WindowHelper::PtInRect(pt.x, pt.y, g_State.holeX + g_State.holeW - 15, g_State.holeY + g_State.holeH - 15, 15, 15)) {
                SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
                return TRUE;
            }

            if (WindowHelper::PtInRect(pt.x, pt.y, g_State.holeX, g_State.holeY, g_State.holeW, g_State.holeH)) {
                SetCursor(LoadCursor(NULL, IDC_SIZEALL));
                return TRUE;
            }

            if (g_State.autoClick) {
                if (WindowHelper::PtInRect(pt.x, pt.y, g_State.dotX, g_State.dotY, g_State.dotSize, g_State.dotSize)) {
                    SetCursor(LoadCursor(NULL, IDC_SIZEALL));
                    return TRUE;
                }
            }
        }
        break;
    }

    case WM_NCHITTEST: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hWnd, &pt);
        return WindowHelper::HandleNCHitTest(hWnd, g_State, pt);
    }

    case WM_LBUTTONDOWN: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        
        if (g_State.autoClick) {
            if (WindowHelper::PtInRect(pt.x, pt.y, g_State.dotX, g_State.dotY, g_State.dotSize, g_State.dotSize)) {
                g_dragMgr->StartDrag(pt.x, pt.y, DRAG_DOT_MOVE);
                SetCapture(hWnd);
                return 0;
            }
        }

        if (WindowHelper::PtInRect(pt.x, pt.y, g_State.holeX, g_State.holeY, g_State.holeW, g_State.holeH)) {
            if (WindowHelper::PtInRect(pt.x, pt.y, g_State.holeX + g_State.holeW - 15, g_State.holeY + g_State.holeH - 15, 15, 15)) {
                g_dragMgr->StartDrag(pt.x, pt.y, DRAG_HOLE_RESIZE);
            } else {
                g_dragMgr->StartDrag(pt.x, pt.y, DRAG_HOLE_MOVE);
            }
            SetCapture(hWnd);
            return 0;
        }
        return 0;
    }

    case WM_MOUSEMOVE: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if (g_dragMgr->GetDragMode() != DRAG_NONE) {
            g_dragMgr->UpdateDrag(pt.x, pt.y, g_State);
            InvalidateRect(hWnd, NULL, FALSE);
        } else if (g_State.holeFollowCursor) {
            // Use current client rect so clamps match actual window size
            RECT rc;
            GetClientRect(hWnd, &rc);
            int clientW = rc.right - rc.left;
            int clientH = rc.bottom - rc.top;

            // Move hole so that mouse is its center
            g_State.holeX = pt.x - g_State.holeW / 2;
            g_State.holeY = pt.y - g_State.holeH / 2;
            if (g_State.holeX < 0) g_State.holeX = 0;
            if (g_State.holeY < 0) g_State.holeY = 0;
            if (g_State.holeX > clientW - g_State.holeW) g_State.holeX = max(0, clientW - g_State.holeW);
            if (g_State.holeY > clientH - g_State.holeH) g_State.holeY = max(0, clientH - g_State.holeH);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }

    case WM_LBUTTONUP:
        if (g_dragMgr->GetDragMode() != DRAG_NONE) {
            ReleaseCapture();
            g_dragMgr->EndDrag();
        }
        return 0;

    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_TRAY_AUTOCLICK) {
            g_State.autoClick = !g_State.autoClick;
            if (g_State.autoClick) {
                g_autoClickMgr->StartTimer(hWnd);
            } else {
                g_autoClickMgr->StopTimer(hWnd);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == ID_TRAY_HOLEFOLLOW) {
            g_State.holeFollowCursor = !g_State.holeFollowCursor;
        }
        else if (LOWORD(wParam) == ID_TRAY_LOCKMOVE) {
            g_State.lockWindowPosition = !g_State.lockWindowPosition;
        }
        else if (LOWORD(wParam) == ID_TRAY_GRAB) {
            g_bgMgr->GrabBackground(hWnd);
        }
        else if (LOWORD(wParam) == ID_TRAY_RESET) {
            g_bgMgr->ResetBackground(hWnd);
        }
        return 0; 
    }

    case WM_TIMER: {
        if (wParam == IDT_AUTOCLICK) {
            g_autoClickMgr->PerformClick(hWnd);
            g_autoClickMgr->ResetTimer(hWnd);
        }
        return 0;
    }

    case WM_RBUTTONUP:
    case WM_NCRBUTTONUP: {
        POINT pt;
        GetCursorPos(&pt);
        g_uiMgr->ShowContextMenu(hWnd, pt);
        return 0;
    }

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            if (g_State.holeFollowCursor) {
                g_State.holeFollowCursor = false;
                InvalidateRect(hWnd, NULL, FALSE);
            } else {
                DestroyWindow(hWnd);
            }
            break;
        case VK_LEFT:
            g_State.holeX -= 2;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case VK_RIGHT:
            g_State.holeX += 2;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case VK_UP:
            g_State.holeY -= 2;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case VK_DOWN:
            g_State.holeY += 2;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        return 0;

    case WM_SIZE: {
        int w = LOWORD(lParam);
        int h = HIWORD(lParam);
        if (w > 0 && h > 0) {
            g_State.winW = w;
            g_State.winH = h;

            int maxHoleX = g_State.winW - g_State.holeW;
            if (maxHoleX < 0) maxHoleX = 0;
            int maxHoleY = g_State.winH - g_State.holeH;
            if (maxHoleY < 0) maxHoleY = 0;
            if (g_State.holeX > maxHoleX) g_State.holeX = maxHoleX;
            if (g_State.holeY > maxHoleY) g_State.holeY = maxHoleY;

            int maxDotX = g_State.winW - g_State.dotSize;
            if (maxDotX < 0) maxDotX = 0;
            int maxDotY = g_State.winH - g_State.dotSize;
            if (maxDotY < 0) maxDotY = 0;
            if (g_State.dotX > maxDotX) g_State.dotX = maxDotX;
            if (g_State.dotY > maxDotY) g_State.dotY = maxDotY;

            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }

    case WM_WINDOWPOSCHANGED: {
        RECT rc;
        GetClientRect(hWnd, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;
        if (w != g_State.winW || h != g_State.winH) {
            g_State.winW = w;
            g_State.winH = h;

            int maxHoleX = g_State.winW - g_State.holeW;
            if (maxHoleX < 0) maxHoleX = 0;
            int maxHoleY = g_State.winH - g_State.holeH;
            if (maxHoleY < 0) maxHoleY = 0;
            if (g_State.holeX > maxHoleX) g_State.holeX = maxHoleX;
            if (g_State.holeY > maxHoleY) g_State.holeY = maxHoleY;

            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }

    case WM_MOUSEWHEEL: {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0) g_State.alpha = min(255, g_State.alpha + 13);
        else g_State.alpha = max(25, g_State.alpha - 13);
        WindowHelper::SetTransparency(g_hWnd, g_State.alpha);
        return 0;
    }

    case WM_DESTROY:
        g_configMgr->SaveConfig();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
