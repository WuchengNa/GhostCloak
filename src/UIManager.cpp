#include "UIManager.h"
#include <windows.h>

#define ID_TRAY_EXIT        1001
#define ID_TRAY_GRAB        1002
#define ID_TRAY_RESET       1003
#define ID_TRAY_AUTOCLICK   1004
#define ID_TRAY_LOCKMOVE    1005
#define ID_TRAY_HOLEFOLLOW  1006

UIManager::UIManager(AppState& state) : m_state(state) {}

void UIManager::ShowContextMenu(HWND hWnd, POINT pt) {
    HMENU hMenu = CreatePopupMenu();
    
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_GRAB, L"Grab Background");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_RESET, L"Reset Background");
    
    if (m_state.autoClick)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_AUTOCLICK, L"Stop Auto Clicker");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_AUTOCLICK, L"Auto Clicker");

    if (m_state.holeFollowCursor)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_HOLEFOLLOW, L"Stop Hole Follow");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_HOLEFOLLOW, L"Hole Follow Cursor");

    if (m_state.lockWindowPosition)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_LOCKMOVE, L"unlock Window Position");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_LOCKMOVE, L"lock Window Position");

    AppendMenuW(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    SetForegroundWindow(hWnd);
    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);

    if (cmd == ID_TRAY_EXIT) DestroyWindow(hWnd);
    else if (cmd == ID_TRAY_GRAB) PostMessage(hWnd, WM_COMMAND, ID_TRAY_GRAB, 0);
    else if (cmd == ID_TRAY_RESET) PostMessage(hWnd, WM_COMMAND, ID_TRAY_RESET, 0);
    else if (cmd == ID_TRAY_AUTOCLICK) PostMessage(hWnd, WM_COMMAND, ID_TRAY_AUTOCLICK, 0);
    else if (cmd == ID_TRAY_HOLEFOLLOW) PostMessage(hWnd, WM_COMMAND, ID_TRAY_HOLEFOLLOW, 0);
    else if (cmd == ID_TRAY_LOCKMOVE) PostMessage(hWnd, WM_COMMAND, ID_TRAY_LOCKMOVE, 0);
}
