#include <windows.h>
#include <commctrl.h>
#include "Editor.h"
#include "resource.h"

Editor g_editor;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            InitCommonControls();
            g_editor.Init(hWnd);
            break;

        case WM_COMMAND:
            g_editor.OnCommand(wParam);
            break;

        case WM_NOTIFY:
            return g_editor.OnNotify(lParam);

        case WM_LBUTTONDOWN:
            g_editor.OnLButtonDown(hWnd, lParam);
            break;

        case WM_MOUSEMOVE:
            g_editor.OnMouseMove(hWnd, lParam);
            break;

        case WM_LBUTTONUP:
            g_editor.OnLButtonUp(hWnd, lParam);
            break;

        case WM_PAINT:
            g_editor.OnPaint(hWnd);
            break;

        case WM_DESTROY:
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
    wc.lpszClassName = L"Lab3WindowClass";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(
        0, L"Lab3WindowClass", L"Лабораторна 3",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}