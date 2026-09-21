#include "module1_step2.h"
#include "resource.h"

static INT_PTR CALLBACK Dialog1_Step2_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_BACK) {
            EndDialog(hDlg, 2); // 2 = повернення на Крок 1
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, 3); // 3 = успішно завершено
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0); // 0 = скасовано
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR ShowDialogStep2(HWND hWndParent) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_B1_STEP2), hWndParent, Dialog1_Step2_Proc);
}