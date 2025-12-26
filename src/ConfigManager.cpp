#include "ConfigManager.h"
#include <windows.h>

ConfigManager::ConfigManager(AppState& state) : m_state(state) {}

void ConfigManager::LoadConfig() {
    // Read raw values from INI (use sensible defaults as fallbacks)
    m_state.winX = ::GetPrivateProfileIntW(L"Window", L"X", -1, m_state.configPath.c_str());
    m_state.winY = ::GetPrivateProfileIntW(L"Window", L"Y", -1, m_state.configPath.c_str());
    m_state.winW = ::GetPrivateProfileIntW(L"Window", L"W", 800, m_state.configPath.c_str());
    m_state.winH = ::GetPrivateProfileIntW(L"Window", L"H", 600, m_state.configPath.c_str());
    m_state.holeX = ::GetPrivateProfileIntW(L"Hole", L"X", 100, m_state.configPath.c_str());
    m_state.holeY = ::GetPrivateProfileIntW(L"Hole", L"Y", 100, m_state.configPath.c_str());
    m_state.holeW = ::GetPrivateProfileIntW(L"Hole", L"W", 300, m_state.configPath.c_str());
    m_state.holeH = ::GetPrivateProfileIntW(L"Hole", L"H", 200, m_state.configPath.c_str());
    int alphaInt = ::GetPrivateProfileIntW(L"State", L"Alpha", 153, m_state.configPath.c_str());
    int bgModeInt = ::GetPrivateProfileIntW(L"State", L"BgMode", 0, m_state.configPath.c_str());
    int lockInt = ::GetPrivateProfileIntW(L"State", L"LockWindowPosition", 0, m_state.configPath.c_str());
    int followInt = ::GetPrivateProfileIntW(L"State", L"HoleFollowCursor", 0, m_state.configPath.c_str());
    m_state.bgMode = (bgModeInt != 0);
    m_state.lockWindowPosition = (lockInt != 0);
    m_state.holeFollowCursor = (followInt != 0);

    // Validate ranges and apply safe defaults when values are out-of-range.
    const int MIN_WIN_W = 200;
    const int MIN_WIN_H = 150;
    const int MAX_DIM = 10000; // arbitrary large cap to catch garbage values
    const int MIN_HOLE = 10;

    const int DEFAULT_WINW = 800;
    const int DEFAULT_WINH = 600;
    const int DEFAULT_HOLEX = 100;
    const int DEFAULT_HOLEY = 100;
    const int DEFAULT_HOLEW = 300;
    const int DEFAULT_HOLEH = 200;
    const int DEFAULT_ALPHA = 153;

    if (m_state.winW < MIN_WIN_W || m_state.winW > MAX_DIM) m_state.winW = DEFAULT_WINW;
    if (m_state.winH < MIN_WIN_H || m_state.winH > MAX_DIM) m_state.winH = DEFAULT_WINH;

    if (m_state.winX < -MAX_DIM || m_state.winX > MAX_DIM) m_state.winX = -1;
    if (m_state.winY < -MAX_DIM || m_state.winY > MAX_DIM) m_state.winY = -1;

    if (m_state.holeW < MIN_HOLE || m_state.holeW > m_state.winW) m_state.holeW = DEFAULT_HOLEW;
    if (m_state.holeH < MIN_HOLE || m_state.holeH > m_state.winH) m_state.holeH = DEFAULT_HOLEH;

    if (m_state.holeX < 0 || m_state.holeX > m_state.winW - MIN_HOLE) m_state.holeX = DEFAULT_HOLEX;
    if (m_state.holeY < 0 || m_state.holeY > m_state.winH - MIN_HOLE) m_state.holeY = DEFAULT_HOLEY;

    if (alphaInt < 0 || alphaInt > 255) m_state.alpha = DEFAULT_ALPHA;
    else m_state.alpha = static_cast<BYTE>(alphaInt);
}

void ConfigManager::SaveConfig() {
    RECT rc;
    GetWindowRect(GetActiveWindow(), &rc);
    
    wchar_t buf[32];
    ::WritePrivateProfileStringW(L"Window", L"X", _itow(rc.left, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Window", L"Y", _itow(rc.top, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Window", L"W", _itow(rc.right - rc.left, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Window", L"H", _itow(rc.bottom - rc.top, buf, 10), m_state.configPath.c_str());

    ::WritePrivateProfileStringW(L"Hole", L"X", _itow(m_state.holeX, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Hole", L"Y", _itow(m_state.holeY, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Hole", L"W", _itow(m_state.holeW, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"Hole", L"H", _itow(m_state.holeH, buf, 10), m_state.configPath.c_str());

    ::WritePrivateProfileStringW(L"State", L"Alpha", _itow(m_state.alpha, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"State", L"BgMode", _itow(m_state.bgMode, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"State", L"LockWindowPosition", _itow(m_state.lockWindowPosition, buf, 10), m_state.configPath.c_str());
    ::WritePrivateProfileStringW(L"State", L"HoleFollowCursor", _itow(m_state.holeFollowCursor, buf, 10), m_state.configPath.c_str());
}
