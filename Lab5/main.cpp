#include <windows.h>
#include <commctrl.h>
#include "editor.h"
#include "my_table.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            MyEditor::getInstance()->Init(hWnd);
            break;

        case WM_SIZE:
            MyEditor::getInstance()->OnSize();
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDM_TABLE) {
                MyTable::getInstance()->Toggle(hWnd);
            } else if (LOWORD(wParam) == IDM_EXPORT) {
                MyTable::getInstance()->ExportToCSV(hWnd);
            } else {
                MyEditor::getInstance()->OnCommand(wParam);
            }
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
            // Автоматично оновлюємо таблицю при додаванні нової фігури
            MyTable::getInstance()->Update();
            break;

        case WM_PAINT:
            MyEditor::getInstance()->OnPaint(hWnd);
            break;

        case WM_DESTROY:
            // Спочатку закриваємо й знищуємо вікно таблиці, потім редактор
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
        // Перевірка повідомлень для немодального діалогового вікна таблиці
        HWND hTableDlg = MyTable::getInstance()->GetHWnd();
        if (!hTableDlg || !IsDialogMessage(hTableDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}