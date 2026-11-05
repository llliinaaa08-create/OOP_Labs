#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <cstdio>

#define WM_USER_OBJ2_DONE (WM_USER + 10)

struct DataParams {
    int n;
    double minVal;
    double maxVal;
};

std::vector<double> g_data;

void CopyToClipboard(const std::string& text) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hMem) {
            memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COPYDATA: {
        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lp;
        if (pcds && pcds->cbData == sizeof(DataParams)) {
            DataParams* p = (DataParams*)pcds->lpData;
            g_data.clear();

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(p->minVal, p->maxVal);

            std::stringstream ss;
            for (int i = 0; i < p->n; ++i) {
                double val = dis(gen);
                g_data.push_back(val);
                ss << std::fixed << std::setprecision(2) << val << " ";
            }

            CopyToClipboard(ss.str());
            InvalidateRect(hWnd, NULL, TRUE);

            HWND hMain = (HWND)wp;
            if (hMain) PostMessage(hMain, WM_USER_OBJ2_DONE, 0, 0);
        }
        return TRUE;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, L"Згенерований масив (Object2):", 29);

        int x = 10, y = 40;
        int colWidth = 70;
        int rowHeight = 20;

        for (size_t i = 0; i < g_data.size(); ++i) {
            wchar_t buf[32];
            swprintf(buf, L"%.2f", g_data[i]);
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
    wc.lpszClassName = L"Object2Class";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"Object2Class", L"Object2 - Генератор даних",
        WS_OVERLAPPEDWINDOW, 390, 100, 420, 400, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}