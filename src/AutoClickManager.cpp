#include "AutoClickManager.h"
#include "Commands.h"
#include <windows.h>
#include <cstdlib>

AutoClickManager::AutoClickManager(AppState& state) : m_state(state) {}

void AutoClickManager::StartTimer(HWND hWnd) {
    if (!m_state.autoClick) {
        return;
    }

    if (m_state.autoClickPoints.empty()) {
        m_state.autoClickPoints.push_back({ m_state.dotX, m_state.dotY, m_state.dotSize, 3000 });
    }

    m_currentPointIndex = 0;
    int interval = GetCurrentPointInterval();
    SetTimer(hWnd, IDT_AUTOCLICK, interval, NULL);
}

void AutoClickManager::StopTimer(HWND hWnd) {
    KillTimer(hWnd, IDT_AUTOCLICK);
}

void AutoClickManager::PerformClick(HWND hWnd) {
    if (m_state.autoClickPoints.empty()) {
        return;
    }

    const AutoClickPoint& point = m_state.autoClickPoints[m_currentPointIndex];
    int randomX = point.x + (rand() % max(1, point.size));
    int randomY = point.y + (rand() % max(1, point.size));
    POINT pt = { randomX, randomY };
    ClientToScreen(hWnd, &pt);

    LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    INPUT inputs[5] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = (LONG)(pt.x * (65535.0f / (sw - 1)));
    inputs[0].mi.dy = (LONG)(pt.y * (65535.0f / (sh - 1)));
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    inputs[3].type = INPUT_MOUSE;
    inputs[3].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[4].type = INPUT_MOUSE;
    inputs[4].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(5, inputs, sizeof(INPUT));
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
}

void AutoClickManager::ResetTimer(HWND hWnd) {
    KillTimer(hWnd, IDT_AUTOCLICK);
    if (!m_state.autoClick || m_state.autoClickPoints.empty()) {
        return;
    }

    AdvanceToNextPoint();
    int interval = GetCurrentPointInterval();
    SetTimer(hWnd, IDT_AUTOCLICK, interval, NULL);
}

int AutoClickManager::GetCurrentPointInterval() const {
    if (m_state.autoClickPoints.empty()) {
        return 3000;
    }
    return max(100, m_state.autoClickPoints[m_currentPointIndex].intervalMs);
}

void AutoClickManager::AdvanceToNextPoint() {
    if (m_state.autoClickPoints.empty()) {
        m_currentPointIndex = 0;
        return;
    }
    m_currentPointIndex = (m_currentPointIndex + 1) % static_cast<int>(m_state.autoClickPoints.size());
}
         
