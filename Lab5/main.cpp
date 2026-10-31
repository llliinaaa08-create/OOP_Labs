#include <windows.h>
#include <commctrl.h>
#include <string>
#include "editor.h"
#include "my_table.h"
#include "resource.h"

// Конкретна реалізація Listener у main.cpp
class MainTableListener : public ITableListener {
public:
    int OnTableGetShapeCount() override {
        return MyEditor::getInstance()->GetShapeCount();
    }

    bool OnTableGetShapeData(int index, wchar_t* nameBuf, int bufSize, int& x1, int& y1, int& x2, int& y2) override {
        Shape* shape = MyEditor::getInstance()->GetShape(index);
        if (!shape) return false;
        shape->GetName(nameBuf, bufSize);
        shape->GetPoints(x1, y1, x2, y2);
        return true;
    }

    void OnTableClearShapes() override {
        MyEditor::getInstance()->ClearShapes();
    }

    void OnTableAddShape(const wchar_t* name, int x1, int y1, int x2, int y2) override {
        Shape* newShape = nullptr;
        std::wstring nameStr(name);

        if (nameStr == L"Крапка" || nameStr == L"Точка") newShape = new PointShape();
        else if (nameStr == L"Лінія") newShape = new LineShape();
        else if (nameStr == L"Прямокутник") newShape = new RectangleShape();
        else if (nameStr == L"Еліпс") newShape = new EllipseShape();
        else if (nameStr == L"Лінія з кружечками") newShape = new LineOOShape();
        else if (nameStr == L"Куб") newShape = new CubeShape();

        if (newShape) {
            newShape->SetPoints(x1, y1, x2, y2);
            MyEditor::getInstance()->AddShape(newShape);
        }
    }
};

static MainTableListener g_tableListener;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            MyEditor::getInstance()->Init(hWnd);
            MyTable::getInstance()->SetListener(&g_tableListener);
            break;

        case WM_SIZE:
            MyEditor::getInstance()->OnSize();
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDM_TABLE) {
                MyTable::getInstance()->Toggle(hWnd);
            } else if (LOWORD(wParam) == IDM_EXPORT) {
                MyTable::getInstance()->ExportToCSV(hWnd);
            } else if (LOWORD(wParam) == IDM_IMPORT) {
                MyTable::getInstance()->ImportFromCSV(hWnd);
            } else {
                MyEditor::getInstance()->OnCommand(wParam);
            }
            break;

        case WM_TABLE_SELECT_SHAPE:
            MyEditor::getInstance()->SetSelectedIndex((int)wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_TABLE_DELETE_SHAPE:
            MyEditor::getInstance()->DeleteShape((int)wParam);
            MyTable::getInstance()->Update();
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_NOTIFY:
            return MyEditor::getInstance()->OnNotify(lParam);

        case WM_LBUTTONDOWN:
            MyEditor::getInstance()->OnLButtonDown(hWnd, lParam);
            break;

        case WM_MOUSEMOVE:
            MyEditor::getInstance()->OnMouseMove(hWnd, lParam);
            break;

        case WM_LBUTTONUP:
            MyEditor::getInstance()->OnLButtonUp(hWnd, lParam);
            MyTable::getInstance()->Update();
            break;

        case WM_PAINT:
            MyEditor::getInstance()->OnPaint(hWnd);
            break;

        case WM_DESTROY:
            MyTable::destroyInstance();
            MyEditor::destroyInstance();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wc.lpszClassName = L"Lab5WindowClass";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(
        0, L"Lab5WindowClass", L"Лабораторна 5",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        HWND hTableDlg = MyTable::getInstance()->GetHWnd();
        if (!hTableDlg || !IsDialogMessage(hTableDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}