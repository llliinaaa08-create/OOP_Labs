#include "module2.h"
#include "resource.h"


static std::wstring* g_pSelectedGroup = nullptr;

static INT_PTR CALLBACK Dialog2_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        // Додаємо назви груп у список при відкритті вікна
        HWND hList = GetDlgItem(hDlg, IDC_LISTBOX);
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)L"Група КН-21");
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)L"Група ПІ-21");
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)L"Група КІ-21");
        SendMessageW(hList, LB_SETCURSEL, 0, 0); // Вибираємо перший елемент за замовчуванням
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) { // Кнопка "Так"
            HWND hList = GetDlgItem(hDlg, IDC_LISTBOX);
            int selIndex = SendMessage(hList, LB_GETCURSEL, 0, 0); // Отримуємо індекс вибраного
            
            if (selIndex != LB_ERR && g_pSelectedGroup != nullptr) {
                wchar_t buffer[256];
                SendMessageW(hList, LB_GETTEXT, selIndex, (LPARAM)buffer); // Отримуємо текст
                *g_pSelectedGroup = buffer; // Записуємо у змінну
                EndDialog(hDlg, 1);
            } else {
                EndDialog(hDlg, 0);
            }
            return (INT_PTR)TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) { // Кнопка "Відміна"
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool ShowDialogModule2(HWND hWndParent, std::wstring& outSelectedGroup) {
    g_pSelectedGroup = &outSelectedGroup;
    HINSTANCE hInst = GetModuleHandle(NULL);
    INT_PTR res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_B2), hWndParent, Dialog2_Proc);
    g_pSelectedGroup = nullptr;
    return (res == 1);
}