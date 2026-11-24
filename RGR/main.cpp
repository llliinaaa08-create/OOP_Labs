#include <windows.h>
#include <commdlg.h>
#include <string>
#include "resource.h"
#include "Analysis.h"
#include "Viewer.h"

DependencyAnalyzer g_analyzer;
TreeViewer g_viewer;
int g_scrollX = 0, g_scrollY = 0;

void UpdateScrollBars(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);

    SCROLLINFO si = { sizeof(si) };
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

    si.nMin = 0;
    si.nMax = g_viewer.GetTotalWidth();
    si.nPage = rc.right;
    si.nPos = g_scrollX;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    si.nMin = 0;
    si.nMax = g_viewer.GetTotalHeight();
    si.nPage = rc.bottom;
    si.nPos = g_scrollY;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_FILE_OPEN: {
            wchar_t szFile[260] = { 0 };
            OPENFILENAMEW ofn = { sizeof(ofn) };
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
            ofn.lpstrFilter = L"C++ Files (*.cpp;*.h)\0*.cpp;*.h\0All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileNameW(&ofn)) {
                char szPathA[260];
                WideCharToMultiByte(CP_ACP, 0, szFile, -1, szPathA, 260, NULL, NULL);

                auto root = g_analyzer.AnalyzeProject(szPathA);
                g_viewer.SetRootNode(root);

                g_scrollX = 0;
                g_scrollY = 0;
                UpdateScrollBars(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
        case IDM_FILE_EXIT:
            DestroyWindow(hWnd);
            break;

        case IDM_HELP_ABOUT:
            MessageBoxW(hWnd, L"РГР Варіант 6: Автоматична побудова #include-ієрархії файлів C++.", L"Про програму", MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;

    case WM_SIZE:
        UpdateScrollBars(hWnd);
        break;

    case WM_HSCROLL: {
        int action = LOWORD(wp);
        if (action == SB_LINELEFT) g_scrollX -= 20;
        else if (action == SB_LINERIGHT) g_scrollX += 20;
        else if (action == SB_PAGELEFT) g_scrollX -= 100;
        else if (action == SB_PAGERIGHT) g_scrollX += 100;
        else if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) g_scrollX = HIWORD(wp);

        if (g_scrollX < 0) g_scrollX = 0;
        UpdateScrollBars(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_VSCROLL: {
        int action = LOWORD(wp);
        if (action == SB_LINEUP) g_scrollY -= 20;
        else if (action == SB_LINEDOWN) g_scrollY += 20;
        else if (action == SB_PAGEUP) g_scrollY -= 100;
        else if (action == SB_PAGEDOWN) g_scrollY += 100;
        else if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) g_scrollY = HIWORD(wp);

        if (g_scrollY < 0) g_scrollY = 0;
        UpdateScrollBars(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));

        g_viewer.Render(hdc, g_scrollX, g_scrollY);

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
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"RGR_IncludeHierarchyClass";
    wc.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINMENU);
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(L"RGR_IncludeHierarchyClass", 
        L"РГР — Побудова #include-ієрархії файлів (Варіант 6)",
        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}