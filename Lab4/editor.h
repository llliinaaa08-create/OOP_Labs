#ifndef EDITOR_H
#define EDITOR_H

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include <windows.h>
#include <commctrl.h>
#include "shape.h"
#include "resource.h"

const int MAX_SHAPES = 106; // Варіант 6: N = 6 + 100 = 106

class MyEditor {
private:
    Shape* pcshape[MAX_SHAPES];
    int shapeCount;
    int currentTool;

    bool isDrawing;
    POINT startPoint;
    POINT currentPoint;

    HWND hWndParent;
    HWND hToolbar;
    HPEN hRubberPen;

    void UpdateWindowTitle();
    void DrawRubberBand(HWND hWnd, POINT pt1, POINT pt2);

public:
    MyEditor();
    ~MyEditor();

    void Init(HWND hWnd);
    void OnSize();

    void OnCommand(WPARAM wParam);
    LRESULT OnNotify(LPARAM lParam);
    void OnLButtonDown(HWND hWnd, LPARAM lParam);
    void OnMouseMove(HWND hWnd, LPARAM lParam);
    void OnLButtonUp(HWND hWnd, LPARAM lParam);
    void OnPaint(HWND hWnd);
};

#endif