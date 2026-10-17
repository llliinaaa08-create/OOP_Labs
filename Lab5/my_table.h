#ifndef MY_TABLE_H
#define MY_TABLE_H

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include <windows.h>
#include <commctrl.h>

class MyTable {
private:
    static MyTable* pInstance;
    HWND hDlg;
    HWND hListView;

    MyTable();
    ~MyTable();
    MyTable(const MyTable&) = delete;
    MyTable& operator=(const MyTable&) = delete;

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void InitListView();

public:
    static MyTable* getInstance();
    static void destroyInstance();

    void Toggle(HWND hWndParent);
    void Update();
    void ExportToCSV(HWND hWndParent);
    void ImportFromCSV(HWND hWndParent); 

    HWND GetHWnd() const { return hDlg; }
};

#endif