#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>
#include <commctrl.h>
#include "Shape.h"
#include "resource.h"

const int MAX_SHAPES = 107; // Варіант 7: N = 7 + 100 = 107

class Editor {
private:
    Shape* pcshape[MAX_SHAPES];
    int shapeCount;
    int currentTool;

    bool isDrawing;
    POINT startPoint;
    POINT currentPoint;

    HWND hWndParent; // Єдине найменування змінної
    HWND hToolbar;
    HPEN hRubberPen;

    void UpdateWindowTitle();
    void DrawRubberBand(HWND hWnd, POINT pt1, POINT pt2);

public:
    Editor();
    ~Editor();

    void Init(HWND hWnd);

    void OnCommand(WPARAM wParam);
    LRESULT OnNotify(LPARAM lParam);
    void OnLButtonDown(HWND hWnd, LPARAM lParam);
    void OnMouseMove(HWND hWnd, LPARAM lParam);
    void OnLButtonUp(HWND hWnd, LPARAM lParam);
    void OnPaint(HWND hWnd);
};

#endif