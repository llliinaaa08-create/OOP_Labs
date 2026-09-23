#include <windows.h>
#include <cmath>
#include <algorithm>
#include "resource.h"
#include "shape.h"
#include "point_shape.h"
#include "line_shape.h"
#include "rect_shape.h"
#include "ellipse_shape.h"

static Shape **pcshape = NULL;
static int shapeCount = 0;
static const int MAX_SHAPES = 106;

static int currentShapeType = IDM_POINT;
static bool isDrawing = false;
static long xStart = 0, yStart = 0, xPrev = 0, yPrev = 0;

void DrawRubberBand(HDC hdc, int shapeType, long x1, long y1, long x2, long y2) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // Синя суцільна лінія
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    int oldROP = SetROP2(hdc, R2_NOTXORPEN);

    switch (shapeType) {
    case IDM_POINT:
        SetPixel(hdc, x1, y1, RGB(0, 0, 255));
        break;
    case IDM_LINE:
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        break;
    case IDM_RECT:
        Rectangle(hdc, std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2));
        break;
    case IDM_ELLIPSE: {
        long rx = std::abs(x2 - x1);
        long ry = std::abs(y2 - y1);
        Ellipse(hdc, x1 - rx, y1 - ry, x1 + rx, y1 + ry);
        break;
    }
    }

    SetROP2(hdc, oldROP);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        pcshape = new Shape*[MAX_SHAPES];
        for (int i = 0; i < MAX_SHAPES; ++i) pcshape[i] = NULL;
        break;

    case WM_INITMENUPOPUP: {
        HMENU hMenu = (HMENU)wParam;
        CheckMenuItem(hMenu, IDM_POINT, MF_BYCOMMAND | (currentShapeType == IDM_POINT ? MF_CHECKED : MF_UNCHECKED));
        CheckMenuItem(hMenu, IDM_LINE, MF_BYCOMMAND | (currentShapeType == IDM_LINE ? MF_CHECKED : MF_UNCHECKED));
        CheckMenuItem(hMenu, IDM_RECT, MF_BYCOMMAND | (currentShapeType == IDM_RECT ? MF_CHECKED : MF_UNCHECKED));
        CheckMenuItem(hMenu, IDM_ELLIPSE, MF_BYCOMMAND | (currentShapeType == IDM_ELLIPSE ? MF_CHECKED : MF_UNCHECKED));
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_POINT:
        case IDM_LINE:
        case IDM_RECT:
        case IDM_ELLIPSE:
            currentShapeType = LOWORD(wParam);
            break;
        case IDM_ABOUT:
            MessageBox(hWnd, L"Графічний редактор об'єктів (Лабораторна №2)\nВаріант Ж = 6", L"Про програму", MB_OK | MB_ICONINFORMATION);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        break;

    case WM_LBUTTONDOWN:
        isDrawing = true;
        xStart = LOWORD(lParam);
        yStart = HIWORD(lParam);
        xPrev = xStart;
        yPrev = yStart;
        SetCapture(hWnd);
        {
            HDC hdc = GetDC(hWnd);
            DrawRubberBand(hdc, currentShapeType, xStart, yStart, xPrev, yPrev);
            ReleaseDC(hWnd, hdc);
        }
        break;

    case WM_MOUSEMOVE:
        if (isDrawing) {
            HDC hdc = GetDC(hWnd);
            DrawRubberBand(hdc, currentShapeType, xStart, yStart, xPrev, yPrev); 
            xPrev = LOWORD(lParam);
            yPrev = HIWORD(lParam);
            DrawRubberBand(hdc, currentShapeType, xStart, yStart, xPrev, yPrev); 
            ReleaseDC(hWnd, hdc);
        }
        break;

    case WM_LBUTTONUP:
        if (isDrawing) {
            HDC hdc = GetDC(hWnd);
            DrawRubberBand(hdc, currentShapeType, xStart, yStart, xPrev, yPrev); 
            ReleaseDC(hWnd, hdc);
            ReleaseCapture();
            isDrawing = false;

            long xEnd = LOWORD(lParam);
            long yEnd = HIWORD(lParam);

            if (shapeCount < MAX_SHAPES) {
                Shape* p = NULL;
                switch (currentShapeType) {
                case IDM_POINT: p = new PointShape(); break;
                case IDM_LINE: p = new LineShape(); break;
                case IDM_RECT: p = new RectangleShape(); break;
                case IDM_ELLIPSE: p = new EllipseShape(); break;
                }
                if (p) {
                    p->Set(xStart, yStart, xEnd, yEnd);
                    pcshape[shapeCount++] = p;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        for (int i = 0; i < shapeCount; ++i) {
            if (pcshape[i]) {
                pcshape[i]->Show(hdc);
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        if (pcshape) {
            for (int i = 0; i < shapeCount; ++i) {
                delete pcshape[i];
            }
            delete[] pcshape;
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0,
                      hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)(COLOR_WINDOW + 1), MAKEINTRESOURCE(IDR_MENU1),
                      L"Lab2Class", NULL };

    if (!RegisterClassEx(&wc)) return 0;

    HWND hWnd = CreateWindow(L"Lab2Class", L"OOP_lab2 (Графічний редактор)",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}