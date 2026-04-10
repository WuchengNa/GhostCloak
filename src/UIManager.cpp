#include "UIManager.h"
#include "Commands.h"
#include <windows.h>

UIManager::UIManager(AppState& state) : m_state(state) {}

void UIManager::ShowContextMenu(HWND hWnd, POINT pt) {
    HMENU hMenu = CreatePopupMenu();
    
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_GRAB, L"Grab Background");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_RESET, L"Reset Background");
    
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_ADD_AUTOCLICK_POINT, L"Add Auto Click Point");
    if (m_state.activeAutoClickPoint >= 0 && m_state.activeAutoClickPoint < static_cast<int>(m_state.autoClickPoints.size())) {
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_REMOVE_AUTOCLICK_POINT, L"Remove Auto Click Point");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

        HMENU hIntervalMenu = CreatePopupMenu();
        AppendMenuW(hIntervalMenu, MF_STRING, ID_TRAY_SET_INTERVAL_1000, L"Interval 1 sec");
        AppendMenuW(hIntervalMenu, MF_STRING, ID_TRAY_SET_INTERVAL_2000, L"Interval 2 sec");
        AppendMenuW(hIntervalMenu, MF_STRING, ID_TRAY_SET_INTERVAL_3000, L"Interval 3 sec");
        AppendMenuW(hIntervalMenu, MF_STRING, ID_TRAY_SET_INTERVAL_5000, L"Interval 5 sec");
        AppendMenuW(hIntervalMenu, MF_STRING, ID_TRAY_SET_INTERVAL_10000, L"Interval 10 sec");
        AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hIntervalMenu), L"Set Point Interval");
    }

    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    if (m_state.autoClick)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_AUTOCLICK, L"Stop Auto Click");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_AUTOCLICK, L"Start Auto Click");

    if (m_state.holeFollowCursor)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_HOLEFOLLOW, L"Stop Hole Follow");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_HOLEFOLLOW, L"Hole Follow Cursor");

    if (m_state.lockWindowPosition)
        AppendMenuW(hMenu, MF_STRING | MF_CHECKED, ID_TRAY_LOCKMOVE, L"unlock Window Position");
    else
        AppendMenuW(hMenu, MF_STRING, ID_TRAY_LOCKMOVE, L"lock Window Position");

    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    SetForegroundWindow(hWnd);
    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);

    if (cmd == ID_TRAY_EXIT) DestroyWindow(hWnd);
    else if (cmd == ID_TRAY_GRAB) PostMessage(hWnd, WM_COMMAND, ID_TRAY_GRAB, 0);
    else if (cmd == ID_TRAY_RESET) PostMessage(hWnd, WM_COMMAND, ID_TRAY_RESET, 0);
    else if (cmd == ID_TRAY_ADD_AUTOCLICK_POINT) PostMessage(hWnd, WM_COMMAND, ID_TRAY_ADD_AUTOCLICK_POINT, 0);
    else if (cmd == ID_TRAY_REMOVE_AUTOCLICK_POINT) PostMessage(hWnd, WM_COMMAND, ID_TRAY_REMOVE_AUTOCLICK_POINT, 0);
    else if (cmd == ID_TRAY_AUTOCLICK) PostMessage(hWnd, WM_COMMAND, ID_TRAY_AUTOCLICK, 0);
    else if (cmd == ID_TRAY_HOLEFOLLOW) PostMessage(hWnd, WM_COMMAND, ID_TRAY_HOLEFOLLOW, 0);
    else if (cmd == ID_TRAY_LOCKMOVE) PostMessage(hWnd, WM_COMMAND, ID_TRAY_LOCKMOVE, 0);
    else if (cmd == ID_TRAY_SET_INTERVAL_1000) PostMessage(hWnd, WM_COMMAND, ID_TRAY_SET_INTERVAL_1000, 0);
    else if (cmd == ID_TRAY_SET_INTERVAL_2000) PostMessage(hWnd, WM_COMMAND, ID_TRAY_SET_INTERVAL_2000, 0);
    else if (cmd == ID_TRAY_SET_INTERVAL_3000) PostMessage(hWnd, WM_COMMAND, ID_TRAY_SET_INTERVAL_3000, 0);
    else if (cmd == ID_TRAY_SET_INTERVAL_5000) PostMessage(hWnd, WM_COMMAND, ID_TRAY_SET_INTERVAL_5000, 0);
    else if (cmd == ID_TRAY_SET_INTERVAL_10000) PostMessage(hWnd, WM_COMMAND, ID_TRAY_SET_INTERVAL_10000, 0);
}
