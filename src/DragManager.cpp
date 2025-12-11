#include "DragManager.h"
#include "AppState.h"

DragManager::DragManager() : m_dragMode(DRAG_NONE), m_lastMouse({ 0 }) {}

void DragManager::StartDrag(int x, int y, DragMode mode) {
    m_dragMode = mode;
    m_lastMouse = { x, y };
}

void DragManager::UpdateDrag(int x, int y, AppState& state) {
    if (m_dragMode == DRAG_NONE) return;
    
    int dx = x - m_lastMouse.x;
    int dy = y - m_lastMouse.y;

    if (m_dragMode == DRAG_HOLE_MOVE) {
        state.holeX += dx;
        state.holeY += dy;
    }
    else if (m_dragMode == DRAG_HOLE_RESIZE) {
        state.holeW = max(50, state.holeW + dx);
        state.holeH = max(50, state.holeH + dy);
    }
    else if (m_dragMode == DRAG_DOT_MOVE) {
        state.dotX += dx;
        state.dotY += dy;
    }
    
    m_lastMouse = { x, y };
}

void DragManager::EndDrag() {
    m_dragMode = DRAG_NONE;
}

DragMode DragManager::GetDragMode() const {
    return m_dragMode;
}
