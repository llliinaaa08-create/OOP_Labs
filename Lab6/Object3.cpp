#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdio>

#define WM_USER_START_OBJ3 (WM_USER + 11)

std::vector<double> g_sortedData;

void ReadFromClipboard() {
    g_sortedData.clear();
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText) {
                std::stringstream ss(pszText);
                double val;
                while (ss >> val) {
                    g_sortedData.push_back(val);
                }
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    std::sort(g_sortedData.begin(), g_sortedData.end());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_USER_START_OBJ3:
        ReadFromClipboard();
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, L"Відсортований масив (Object3):", 30);

        int x = 10, y = 40;
        int colWidth = 70;
        int rowHeight = 20;

        for (size_t i = 0; i < g_sortedData.size(); ++i) {
            wchar_t buf[32];
            swprintf(buf, L"%.2f", g_sortedData[i]);
            TextOut(hdc, x, y, buf, wcslen(buf));

            y += rowHeight;
            if (y > 320) {
                y = 40;
                x += colWidth;
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"Object3Class";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"Object3Class", L"Object3 - Сортування",
        WS_OVERLAPPEDWINDOW, 830, 100, 420, 400, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}