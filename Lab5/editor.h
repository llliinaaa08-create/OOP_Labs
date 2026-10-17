#ifndef EDITOR_H
#define EDITOR_H

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include <windows.h>
#include <commctrl.h>
#include "shape.h"
#include "resource.h"

const int MAX_SHAPES = 106;

class MyEditor {
private:
    static MyEditor* pInstance;

    Shape* pcshape[MAX_SHAPES];
    int shapeCount;
    int currentTool;
    int selectedIndex; 

    bool isDrawing;
    POINT startPoint;
    POINT currentPoint;

    HWND hWndParent;
    HWND hToolbar;
    HPEN hRubberPen;

    MyEditor();
    ~MyEditor();
    MyEditor(const MyEditor&) = delete;
    MyEditor& operator=(const MyEditor&) = delete;

    void UpdateWindowTitle();
    void DrawRubberBand(HWND hWnd, POINT pt1, POINT pt2);

public:
    static MyEditor* getInstance();
    static void destroyInstance();

    void Init(HWND hWnd);
    void OnSize();

    void OnCommand(WPARAM wParam);
    LRESULT OnNotify(LPARAM lParam);
    void OnLButtonDown(HWND hWnd, LPARAM lParam);
    void OnMouseMove(HWND hWnd, LPARAM lParam);
    void OnLButtonUp(HWND hWnd, LPARAM lParam);
    void OnPaint(HWND hWnd);

    int GetShapeCount() const { return shapeCount; }
    Shape* GetShape(int index) const {
        if (index >= 0 && index < shapeCount) return pcshape[index];
        return nullptr;
    }

    void SetSelectedIndex(int index) { selectedIndex = index; }
    void DeleteShape(int index);
    void ClearShapes();
    void AddShape(Shape* pShape);
};

#endif