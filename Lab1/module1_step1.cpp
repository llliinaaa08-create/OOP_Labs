#include "module1_step1.h"
#include "resource.h"

static INT_PTR CALLBACK Dialog1_Step1_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_NEXT) {
            EndDialog(hDlg, 1); // 1 = перехід на Крок 2
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0); // 0 = скасовано
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR ShowDialogStep1(HWND hWndParent) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_B1_STEP1), hWndParent, Dialog1_Step1_Proc);
}