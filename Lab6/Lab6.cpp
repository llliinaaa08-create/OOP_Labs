#include <windows.h>
#include <string>

#define ID_BTN_EXECUTE 101
#define ID_EDIT_N       102
#define ID_EDIT_MIN     103
#define ID_EDIT_MAX     104

#define WM_USER_START_OBJ3 (WM_USER + 11)

struct DataParams {
    int n;
    double minVal;
    double maxVal;
};

HWND hEditN, hEditMin, hEditMax;
HWND hObj2 = NULL, hObj3 = NULL;

void LaunchProcessIfNeeded(const wchar_t* exeName, const wchar_t* className, HWND& hWnd) {
    hWnd = FindWindow(className, NULL);
    if (!hWnd) {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(exeName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            int timeout = 0;
            while (!hWnd && timeout < 50) {
                Sleep(100);
                hWnd = FindWindow(className, NULL);
                timeout++;
            }
        }
    }
}

void AlignWindows(HWND hMain, HWND h2, HWND h3) {
    SetWindowPos(hMain, NULL, 50, 100, 320, 240, SWP_NOZORDER);
    if (h2) SetWindowPos(h2, NULL, 390, 100, 420, 400, SWP_NOZORDER | SWP_SHOWWINDOW);
    if (h3) SetWindowPos(h3, NULL, 830, 100, 420, 400, SWP_NOZORDER | SWP_SHOWWINDOW);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"Кількість n:", WS_CHILD | WS_VISIBLE, 20, 20, 100, 20, hWnd, NULL, NULL, NULL);
        hEditN = CreateWindow(L"EDIT", L"20", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 130, 20, 140, 20, hWnd, (HMENU)ID_EDIT_N, NULL, NULL);

        CreateWindow(L"STATIC", L"Min:", WS_CHILD | WS_VISIBLE, 20, 50, 100, 20, hWnd, NULL, NULL, NULL);
        hEditMin = CreateWindow(L"EDIT", L"-10.5", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 50, 140, 20, hWnd, (HMENU)ID_EDIT_MIN, NULL, NULL);

        CreateWindow(L"STATIC", L"Max:", WS_CHILD | WS_VISIBLE, 20, 80, 100, 20, hWnd, NULL, NULL, NULL);
        hEditMax = CreateWindow(L"EDIT", L"50.5", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 80, 140, 20, hWnd, (HMENU)ID_EDIT_MAX, NULL, NULL);

        CreateWindow(L"BUTTON", L"Виконати", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 120, 250, 35, hWnd, (HMENU)ID_BTN_EXECUTE, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wp) == ID_BTN_EXECUTE) {
            wchar_t bufN[32], bufMin[32], bufMax[32];
            GetWindowText(hEditN, bufN, 32);
            GetWindowText(hEditMin, bufMin, 32);
            GetWindowText(hEditMax, bufMax, 32);

            DataParams params;
            params.n = _wtoi(bufN);
            params.minVal = _wtof(bufMin);
            params.maxVal = _wtof(bufMax);

            LaunchProcessIfNeeded(L"Object2.exe", L"Object2Class", hObj2);
            LaunchProcessIfNeeded(L"Object3.exe", L"Object3Class", hObj3);

            AlignWindows(hWnd, hObj2, hObj3);

            if (hObj2) {
                COPYDATASTRUCT cds;
                cds.dwData = 1;
                cds.cbData = sizeof(params);
                cds.lpData = &params;
                SendMessage(hObj2, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
            }

            if (hObj3) {
                Sleep(50);
                PostMessage(hObj3, WM_USER_START_OBJ3, 0, 0);
            }
        }
        break;

    case WM_DESTROY:
        if (hObj2) PostMessage(hObj2, WM_CLOSE, 0, 0);
        if (hObj3) PostMessage(hObj3, WM_CLOSE, 0, 0);
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
    wc.lpszClassName = L"Lab6Class";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"Lab6Class", L"Lab6 - Менеджер (Варіант 2)", 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 50, 100, 320, 240, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}