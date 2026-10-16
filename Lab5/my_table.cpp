#include "my_table.h"
#include "editor.h"
#include "resource.h"
#include <fstream>

MyTable* MyTable::pInstance = nullptr;

MyTable* MyTable::getInstance() {
    if (!pInstance) {
        pInstance = new MyTable();
    }
    return pInstance;
}

void MyTable::destroyInstance() {
    if (pInstance) {
        delete pInstance;
        pInstance = nullptr;
    }
}

MyTable::MyTable() {
    hDlg = NULL;
    hListView = NULL;
}

MyTable::~MyTable() {
    if (hDlg) {
        DestroyWindow(hDlg);
        hDlg = NULL;
    }
}

void MyTable::Toggle(HWND hWndParent) {
    if (!hDlg) {
        hDlg = CreateDialogParam(
            GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDD_TABLE_DIALOG),
            hWndParent,
            DlgProc,
            (LPARAM)this
        );
        ShowWindow(hDlg, SW_SHOW);
    } else {
        if (IsWindowVisible(hDlg)) {
            ShowWindow(hDlg, SW_HIDE);
        } else {
            ShowWindow(hDlg, SW_SHOW);
            Update();
        }
    }
}

INT_PTR CALLBACK MyTable::DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            MyTable* pTable = (MyTable*)lParam;
            SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)pTable);
            pTable->hDlg = hDlg;
            pTable->hListView = GetDlgItem(hDlg, IDC_LIST_TABLE);
            pTable->InitListView();
            pTable->Update();
            return TRUE;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDOK) {
                ShowWindow(hDlg, SW_HIDE);
                return TRUE;
            }
            break;

        case WM_CLOSE:
            ShowWindow(hDlg, SW_HIDE);
            return TRUE;

        case WM_NCDESTROY:
            return TRUE;
    }
    return FALSE;
}

void MyTable::InitListView() {
    if (!hListView) return;

    ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    const wchar_t* headers[] = { L"Назва", L"x1", L"y1", L"x2", L"y2" };
    int widths[] = { 140, 60, 60, 60, 60 };

    for (int i = 0; i < 5; ++i) {
        lvc.iSubItem = i;
        lvc.pszText = (LPWSTR)headers[i];
        lvc.cx = widths[i];
        ListView_InsertColumn(hListView, i, &lvc);
    }
}

void MyTable::Update() {
    if (!hDlg || !IsWindowVisible(hDlg) || !hListView) return;

    ListView_DeleteAllItems(hListView);

    MyEditor* editor = MyEditor::getInstance();
    int count = editor->GetShapeCount();

    for (int i = 0; i < count; ++i) {
        Shape* shape = editor->GetShape(i);
        if (!shape) continue;

        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;

        wchar_t nameBuf[64] = L"Фігура";
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

        shape->GetName(nameBuf, 64);
        shape->GetPoints(x1, y1, x2, y2);

        lvi.pszText = nameBuf;
        ListView_InsertItem(hListView, &lvi);

        wchar_t strBuf[32];

        wsprintf(strBuf, L"%d", x1);
        ListView_SetItemText(hListView, i, 1, strBuf);

        wsprintf(strBuf, L"%d", y1);
        ListView_SetItemText(hListView, i, 2, strBuf);

        wsprintf(strBuf, L"%d", x2);
        ListView_SetItemText(hListView, i, 3, strBuf);

        wsprintf(strBuf, L"%d", y2);
        ListView_SetItemText(hListView, i, 4, strBuf);
    }
}

void MyTable::ExportToCSV(HWND hWndParent) {
    wchar_t szFile[260] = L"shapes.csv";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWndParent;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
    ofn.lpstrFilter = L"CSV файли (*.csv)\0*.csv\0Усі файли (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) {
        char szFileA[260];
        WideCharToMultiByte(CP_ACP, 0, szFile, -1, szFileA, 260, NULL, NULL);

        std::wofstream outFile(szFileA);
        if (outFile.is_open()) {
            outFile << L"Назва,x1,y1,x2,y2\n";

            MyEditor* editor = MyEditor::getInstance();
            int count = editor->GetShapeCount();

            for (int i = 0; i < count; ++i) {
                Shape* shape = editor->GetShape(i);
                if (!shape) continue;

                wchar_t nameBuf[64] = L"";
                int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

                shape->GetName(nameBuf, 64);
                shape->GetPoints(x1, y1, x2, y2);

                outFile << nameBuf << L"," << x1 << L"," << y1 << L"," << x2 << L"," << y2 << L"\n";
            }
            outFile.close();
            MessageBox(hWndParent, L"Дані успішно збережено в CSV файл!", L"Експорт", MB_OK | MB_ICONINFORMATION);
        }
    }
}