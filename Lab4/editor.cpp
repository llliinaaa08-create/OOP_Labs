#include "editor.h"

MyEditor::MyEditor() {
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

MyEditor::~MyEditor() {
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

void MyEditor::Init(HWND hWnd) {
    hWndParent = hWnd;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    hToolbar = CreateToolbarEx(
        hWnd,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        1, 6, GetModuleHandle(NULL), IDB_TOOLBAR,
        NULL, 0, 16, 16, 16, 16, sizeof(TBBUTTON)
    );

    if (hToolbar) {
        SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

        TBBUTTON tbb[6] = {
            { 0, IDM_POINT,     TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
            { 1, IDM_LINE,      TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
            { 2, IDM_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
            { 3, IDM_ELLIPSE,   TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
            { 4, IDM_LINEOO,    TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
            { 5, IDM_CUBE,      TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 }
        };

        SendMessage(hToolbar, TB_ADDBUTTONS, 6, (LPARAM)&tbb);
        SendMessage(hToolbar, TB_AUTOSIZE, 0, 0);
        ShowWindow(hToolbar, SW_SHOW);
    }

    UpdateWindowTitle();
}

void MyEditor::OnSize() {
    if (hToolbar) {
        SendMessage(hToolbar, TB_AUTOSIZE, 0, 0);
    }
}

void MyEditor::UpdateWindowTitle() {
    const wchar_t* toolName = L"Крапка";
    switch (currentTool) {
        case IDM_POINT:     toolName = L"Крапка"; break;
        case IDM_LINE:      toolName = L"Лінія"; break;
        case IDM_RECTANGLE: toolName = L"Прямокутник"; break;
        case IDM_ELLIPSE:   toolName = L"Еліпс"; break;
        case IDM_LINEOO:    toolName = L"Лінія з кружечками"; break;
        case IDM_CUBE:      toolName = L"Куб"; break;
    }

    wchar_t title[128];
    wsprintf(title, L"Лабораторна 4 - [Поточний інструмент: %s]", toolName);
    SetWindowText(hWndParent, title);
}

void MyEditor::OnCommand(WPARAM wParam) {
    int id = LOWORD(wParam);
    if (id >= IDM_POINT && id <= IDM_CUBE) {
        currentTool = id;
        UpdateWindowTitle();
    } else if (id == IDM_ABOUT) {
        MessageBox(hWndParent, L"Лабораторна робота №4\nТема: Ієрархія класів та віртуальне успадкування", L"Про програму", MB_OK | MB_ICONINFORMATION);
    } else if (id == IDM_EXIT) {
        PostQuitMessage(0);
    }
}

LRESULT MyEditor::OnNotify(LPARAM lParam) {
    LPNMHDR pnmh = (LPNMHDR)lParam;
    if (pnmh->code == TTN_GETDISPINFO) {
        NMTTDISPINFOW* pttdi = (NMTTDISPINFOW*)lParam;
        switch (pttdi->hdr.idFrom) {
            case IDM_POINT:     pttdi->lpszText = (LPWSTR)L"Малювати крапку"; break;
            case IDM_LINE:      pttdi->lpszText = (LPWSTR)L"Малювати лінію"; break;
            case IDM_RECTANGLE: pttdi->lpszText = (LPWSTR)L"Малювати прямокутник"; break;
            case IDM_ELLIPSE:   pttdi->lpszText = (LPWSTR)L"Малювати еліпс"; break;
            case IDM_LINEOO:    pttdi->lpszText = (LPWSTR)L"Малювати лінію з кружечками"; break;
            case IDM_CUBE:      pttdi->lpszText = (LPWSTR)L"Малювати каркас куба"; break;
        }
    }
    return 0;
}

void MyEditor::OnLButtonDown(HWND hWnd, LPARAM lParam) {
    isDrawing = true;
    startPoint.x = LOWORD(lParam);
    startPoint.y = HIWORD(lParam);
    currentPoint = startPoint;

    hRubberPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));

    SetCapture(hWnd);
}

void MyEditor::DrawRubberBand(HWND hWnd, POINT pt1, POINT pt2) {
    HDC hdc = GetDC(hWnd);
    int oldROP = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hRubberPen);
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

    if (currentTool == IDM_LINE || currentTool == IDM_LINEOO) {
        MoveToEx(hdc, pt1.x, pt1.y, NULL);
        LineTo(hdc, pt2.x, pt2.y);
    } else if (currentTool == IDM_RECTANGLE || currentTool == IDM_CUBE) {
        Rectangle(hdc, pt1.x, pt1.y, pt2.x, pt2.y);
    } else if (currentTool == IDM_ELLIPSE) {
        Ellipse(hdc, pt1.x, pt1.y, pt2.x, pt2.y);
    }

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    SetROP2(hdc, oldROP);
    ReleaseDC(hWnd, hdc);
}

void MyEditor::OnMouseMove(HWND hWnd, LPARAM lParam) {
    if (!isDrawing || currentTool == IDM_POINT) return;

    DrawRubberBand(hWnd, startPoint, currentPoint);
    currentPoint.x = LOWORD(lParam);
    currentPoint.y = HIWORD(lParam);
    DrawRubberBand(hWnd, startPoint, currentPoint);
}

void MyEditor::OnLButtonUp(HWND hWnd, LPARAM lParam) {
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
        case IDM_LINEOO:    newShape = new LineOOShape(); break;
        case IDM_CUBE:      newShape = new CubeShape(); break;
    }

    if (newShape) {
        newShape->SetPoints(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        pcshape[shapeCount++] = newShape;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void MyEditor::OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < shapeCount; ++i) {
        if (pcshape[i]) {
            pcshape[i]->Draw(hdc);
        }
    }

    EndPaint(hWnd, &ps);
}