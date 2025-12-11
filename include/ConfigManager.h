#pragma once
#include <windows.h>
#include <string>
#include "AppState.h"

class ConfigManager {
public:
    ConfigManager(AppState& state);
    
    void LoadConfig();
    void SaveConfig();
    
private:
    AppState& m_state;
    
    int GetPrivateProfileIntW(const wchar_t* appName, const wchar_t* key, int defaultValue);
    void WritePrivateProfileStringW(const wchar_t* appName, const wchar_t* key, const wchar_t* value);
};
