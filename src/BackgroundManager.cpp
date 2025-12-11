#include "BackgroundManager.h"
#include <windows.h>

BackgroundManager::BackgroundManager(AppState& state) : m_state(state), m_bgImage(NULL) {}

BackgroundManager::~BackgroundManager() {
    if (m_bgImage) delete m_bgImage;
}

void BackgroundManager::GrabBackground(HWND hWnd) {
    // 1. Hide Window
    ShowWindow(hWnd, SW_HIDE);
    Sleep(500);

    // 2. Capture screen
    RECT rc;
    GetWindowRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    HDC hScreenDC = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, w, h);
    SelectObject(hMemDC, hBitmap);

    BitBlt(hMemDC, 0, 0, w, h, hScreenDC, rc.left, rc.top, SRCCOPY);

    // 3. Save to GDI+ Bitmap & File
    if (m_bgImage) delete m_bgImage;
    m_bgImage = Bitmap::FromHBITMAP(hBitmap, NULL);

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    m_bgImage->Save(m_state.cachePath.c_str(), &pngClsid, NULL);

    // 4. Cleanup GDI
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    // 5. Restore Window
    m_state.bgMode = true;
    m_state.alpha = 242;
    SetLayeredWindowAttributes(hWnd, RGB(0, 255, 0), m_state.alpha, LWA_COLORKEY | LWA_ALPHA);

    ShowWindow(hWnd, SW_SHOW);
    InvalidateRect(hWnd, NULL, TRUE);
}

void BackgroundManager::RestoreBackground() {
    if (m_bgImage) delete m_bgImage;
    m_bgImage = Image::FromFile(m_state.cachePath.c_str());
    if (m_bgImage && m_bgImage->GetLastStatus() == Ok) {
        m_state.bgMode = true;
    } else {
        if (m_bgImage) delete m_bgImage;
        m_bgImage = NULL;
        m_state.bgMode = false;
    }
}

void BackgroundManager::ResetBackground(HWND hWnd) {
    if (m_bgImage) {
        delete m_bgImage;
        m_bgImage = NULL;
    }
    DeleteFileW(m_state.cachePath.c_str());
    m_state.bgMode = false;
    m_state.alpha = 153;
    SetLayeredWindowAttributes(hWnd, RGB(0, 255, 0), m_state.alpha, LWA_COLORKEY | LWA_ALPHA);
    InvalidateRect(hWnd, NULL, TRUE);
}

Image* BackgroundManager::GetBackgroundImage() const {
    return m_bgImage;
}

int BackgroundManager::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0, size = 0;
    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;
    
    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;
    
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}
