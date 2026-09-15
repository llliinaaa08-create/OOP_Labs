#include <windows.h>
#include <string>
#include "resource.h"
#include "module1.h"
#include "module2.h"

// Текст, який буде виводитися у головному вікні
static std::wstring g_displayText = L"Оберіть завдання з меню.";

// Обробник повідомлень головного вікна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_WORK1: // Натиснуто меню "Робота 1"
            if (ShowDialogModule1(hWnd)) {
                g_displayText = L"Робота 1 (Два вікна) завершена успішно!";
            } else {
                g_displayText = L"Робота 1 була скасована.";
            }
            InvalidateRect(hWnd, NULL, TRUE); // Даємо команду перемалювати вікно
            break;
            
        case IDM_WORK2: { // Натиснуто меню "Робота 2"
            std::wstring selectedGroup;
            if (ShowDialogModule2(hWnd, selectedGroup)) {
                g_displayText = L"Вибрана група: " + selectedGroup; // Завдання B2
            } else {
                g_displayText = L"Вибір групи скасовано.";
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Виводимо текст на екран
        TextOutW(hdc, 20, 20, g_displayText.c_str(), g_displayText.length());
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Точка входу в програму
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Реєстрація класу вікна (Тут ми прикріплюємо меню IDR_MENU1)
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0,
                      hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)(COLOR_WINDOW + 1), MAKEINTRESOURCE(IDR_MENU1), L"Lab1Class", NULL };
    RegisterClassEx(&wc);

    // Створення головного вікна
    HWND hWnd = CreateWindow(L"Lab1Class", L"Лабораторна робота №1 (Ж=6)",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Цикл очікування повідомлень
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}