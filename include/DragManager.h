#pragma once
#include <windows.h>
#include "AppState.h"

enum DragMode {
    DRAG_NONE,
    DRAG_WINDOW_MOVE,
    DRAG_WINDOW_RESIZE_L, DRAG_WINDOW_RESIZE_R, DRAG_WINDOW_RESIZE_T, DRAG_WINDOW_RESIZE_B,
    DRAG_WINDOW_RESIZE_TL, DRAG_WINDOW_RESIZE_TR, DRAG_WINDOW_RESIZE_BL, DRAG_WINDOW_RESIZE_BR,
    DRAG_HOLE_MOVE,
    DRAG_HOLE_RESIZE,
    DRAG_DOT_MOVE
};

class DragManager {
public:
    DragManager();
    
    void StartDrag(int x, int y, DragMode mode);
    void UpdateDrag(int x, int y, AppState& state);
    void EndDrag();
    
    DragMode GetDragMode() const;
    
private:
    DragMode m_dragMode = DRAG_NONE;
    POINT m_lastMouse = { 0 };
};
