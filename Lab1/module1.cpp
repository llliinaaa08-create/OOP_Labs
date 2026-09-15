#include "module1.h"
#include "resource.h"

// Оголошуємо обробник другого вікна заздалегідь
static INT_PTR CALLBACK Dialog1_Step2_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Обробник першого вікна
static INT_PTR CALLBACK Dialog1_Step1_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_NEXT) {
            EndDialog(hDlg, 1); // 1 = йдемо далі
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0); // 0 = відміна
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Обробник другого вікна
static INT_PTR CALLBACK Dialog1_Step2_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_BACK) {
            EndDialog(hDlg, 2); // 2 = йдемо назад
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, 3); // 3 = успішно завершено
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0); // 0 = відміна
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Головна функція, яка керує перемиканням вікон
bool ShowDialogModule1(HWND hWndParent) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    int step = 1; // Починаємо з першого вікна
    
    
    while (step != 0 && step != 3) {
        if (step == 1) {
            INT_PTR res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_B1_STEP1), hWndParent, Dialog1_Step1_Proc);
            if (res == 1) step = 2; 
            else step = 0; 
        } else if (step == 2) {
            INT_PTR res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_B1_STEP2), hWndParent, Dialog1_Step2_Proc);
            if (res == 2) step = 1; 
            else if (res == 3) step = 3;
            else step = 0; 
        }
    }
    
    return (step == 3); // Поверне true, якщо все пройшли до кінця
}