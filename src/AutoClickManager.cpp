#include "AutoClickManager.h"
#include <windows.h>
#include <cstdlib>

#define IDT_AUTOCLICK 999

AutoClickManager::AutoClickManager(AppState& state) : m_state(state) {}

void AutoClickManager::StartTimer(HWND hWnd) {
    if (m_state.autoClick) {
        int interval = GenerateRandomInterval();
        SetTimer(hWnd, IDT_AUTOCLICK, interval, NULL);
    }
}

void AutoClickManager::StopTimer(HWND hWnd) {
    KillTimer(hWnd, IDT_AUTOCLICK);
}

void AutoClickManager::PerformClick(HWND hWnd) {
    // Generate random coordinates within dot range
    int randomX = m_state.dotX + (rand() % m_state.dotSize);
    int randomY = m_state.dotY + (rand() % m_state.dotSize);
    POINT pt = { randomX, randomY };
    ClientToScreen(hWnd, &pt);

    // Temporarily make window transparent to input
    LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);

    // Map screen coords to absolute mouse coords (0-65535)
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    
    INPUT inputs[3] = {};
    // Move
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = (LONG)(pt.x * (65535.0f / (sw - 1)));
    inputs[0].mi.dy = (LONG)(pt.y * (65535.0f / (sh - 1)));
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    
    // Down
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    
    // Up
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(3, inputs, sizeof(INPUT));

    // Restore window style
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
}

void AutoClickManager::ResetTimer(HWND hWnd) {
    KillTimer(hWnd, IDT_AUTOCLICK);
    if (m_state.autoClick) {
        int interval = GenerateRandomInterval();
        SetTimer(hWnd, IDT_AUTOCLICK, interval, NULL);
    }
}

int AutoClickManager::GenerateRandomInterval() {
    return (rand() % 7000) + 3000; // 3-20s
}
         
