#include "Editor.h"

Editor::Editor() {
    shapeCount = 0;
    currentTool = IDM_POINT;
    isDrawing = false;
    hToolbar = NULL;
    hWndParent = NULL;
    hRubberPen = NULL;

    for (int i = 0; i < MAX_SHAPES; ++i) {
        pcshape[i] = NULL;
    }
}

Editor::~Editor() {
    for (int i = 0; i < shapeCount; ++i) {
        if (pcshape[i]) {
            delete pcshape[i];
            pcshape[i] = NULL;
        }
    }
    if (hRubberPen) {
        DeleteObject(hRubberPen);
    }
}

void Editor::Init(HWND hWnd) {
    hWndParent = hWnd;

    TBADDBITMAP tbab;
    tbab.hInst = GetModuleHandle(NULL);
    tbab.nID = IDB_TOOLBAR;

    hToolbar = CreateToolbarEx(
        hWnd,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        1, 5, GetModuleHandle(NULL), IDB_TOOLBAR,
        NULL, 0, 16, 16, 16, 16, sizeof(TBBUTTON)
    );

    TBBUTTON tbb[4] = {
        { 0, IDM_POINT,     TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { 1, IDM_LINE,      TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { 2, IDM_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { 3, IDM_ELLIPSE,   TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 }
    };

    SendMessage(hToolbar, TB_ADDBUTTONS, 4, (LPARAM)&tbb);

    UpdateWindowTitle();
}

void Editor::UpdateWindowTitle() {
    const wchar_t* toolName = L"Крапка";
    switch (currentTool) {
        case IDM_POINT:     toolName = L"Крапка"; break;
        case IDM_LINE:      toolName = L"Лінія"; break;
        case IDM_RECTANGLE: toolName = L"Прямокутник"; break;
        case IDM_ELLIPSE:   toolName = L"Еліпс"; break;
    }

    wchar_t title[128];
    wsprintf(title, L"Лабораторна 3 - [Поточний інструмент: %s]", toolName);
    SetWindowText(hWndParent, title);
}

void Editor::OnCommand(WPARAM wParam) {
    int id = LOWORD(wParam);
    if (id >= IDM_POINT && id <= IDM_ELLIPSE) {
        currentTool = id;
        UpdateWindowTitle();
    } else if (id == IDM_EXIT) {
        PostQuitMessage(0);
    }
}


LRESULT Editor::OnNotify(LPARAM lParam) {
    LPNMHDR pnmh = (LPNMHDR)lParam;
    if (pnmh->code == TTN_GETDISPINFO) {
        NMTTDISPINFOW* pttdi = (NMTTDISPINFOW*)lParam;
        switch (pttdi->hdr.idFrom) {
            case IDM_POINT:     pttdi->lpszText = (LPWSTR)L"Малювати крапку"; break;
            case IDM_LINE:      pttdi->lpszText = (LPWSTR)L"Малювати лінію"; break;
            case IDM_RECTANGLE: pttdi->lpszText = (LPWSTR)L"Малювати прямокутник"; break;
            case IDM_ELLIPSE:   pttdi->lpszText = (LPWSTR)L"Малювати еліпс"; break;
        }
    }
    return 0;
}

// Виділення пера 1 раз при DOWN (виправлення зауваження)
void Editor::OnLButtonDown(HWND hWnd, LPARAM lParam) {
    isDrawing = true;
    startPoint.x = LOWORD(lParam);
    startPoint.y = HIWORD(lParam);
    currentPoint = startPoint;

    // Пунктирне чорне перо (7 mod 4 = 3)
    hRubberPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));

    SetCapture(hWnd);
}

void Editor::DrawRubberBand(HWND hWnd, POINT pt1, POINT pt2) {
    HDC hdc = GetDC(hWnd);
    int oldROP = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hRubberPen);

    if (currentTool == IDM_LINE) {
        MoveToEx(hdc, pt1.x, pt1.y, NULL);
        LineTo(hdc, pt2.x, pt2.y);
    } else if (currentTool == IDM_RECTANGLE) {
        long dx = std::abs(pt2.x - pt1.x);
        long dy = std::abs(pt2.y - pt1.y);
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        Rectangle(hdc, pt1.x - dx, pt1.y - dy, pt1.x + dx, pt1.y + dy);
        SelectObject(hdc, hOldBrush);
    } else if (currentTool == IDM_ELLIPSE) {
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        Ellipse(hdc, pt1.x, pt1.y, pt2.x, pt2.y);
        SelectObject(hdc, hOldBrush);
    }

    SelectObject(hdc, hOldPen);
    SetROP2(hdc, oldROP);
    ReleaseDC(hWnd, hdc);
}

void Editor::OnMouseMove(HWND hWnd, LPARAM lParam) {
    if (!isDrawing || currentTool == IDM_POINT) return;

    DrawRubberBand(hWnd, startPoint, currentPoint);
    currentPoint.x = LOWORD(lParam);
    currentPoint.y = HIWORD(lParam);
    DrawRubberBand(hWnd, startPoint, currentPoint);
}

// Видалення пера в UP
void Editor::OnLButtonUp(HWND hWnd, LPARAM lParam) {
    if (!isDrawing) return;

    if (currentTool != IDM_POINT) {
        DrawRubberBand(hWnd, startPoint, currentPoint);
    }

    ReleaseCapture();
    isDrawing = false;

    if (hRubberPen) {
        DeleteObject(hRubberPen);
        hRubberPen = NULL;
    }

    if (shapeCount >= MAX_SHAPES) return;

    POINT endPoint = { LOWORD(lParam), HIWORD(lParam) };
    Shape* newShape = NULL;

    switch (currentTool) {
        case IDM_POINT:     newShape = new PointShape(); break;
        case IDM_LINE:      newShape = new LineShape(); break;
        case IDM_RECTANGLE: newShape = new RectangleShape(); break;
        case IDM_ELLIPSE:   newShape = new EllipseShape(); break;
    }

    if (newShape) {
        newShape->SetPoints(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        pcshape[shapeCount++] = newShape;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void Editor::OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < shapeCount; ++i) {
        if (pcshape[i]) {
            pcshape[i]->Draw(hdc);
        }
    }

    EndPaint(hWnd, &ps);
}