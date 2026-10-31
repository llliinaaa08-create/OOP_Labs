#ifndef MY_TABLE_H
#define MY_TABLE_H

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include <windows.h>
#include <commctrl.h>

// Інтерфейс Слухача для зв'язку з main.cpp без жорстких залежностей
class ITableListener {
public:
    virtual ~ITableListener() {}
    virtual int OnTableGetShapeCount() = 0;
    virtual bool OnTableGetShapeData(int index, wchar_t* nameBuf, int bufSize, int& x1, int& y1, int& x2, int& y2) = 0;
    virtual void OnTableClearShapes() = 0;
    virtual void OnTableAddShape(const wchar_t* name, int x1, int y1, int x2, int y2) = 0;
};

class MyTable {
private:
    static MyTable* pInstance;
    HWND hDlg;
    HWND hListView;
    ITableListener* pListener; // Вказівник на Listener

    MyTable();
    ~MyTable();
    MyTable(const MyTable&) = delete;
    MyTable& operator=(const MyTable&) = delete;

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void InitListView();

public:
    static MyTable* getInstance();
    static void destroyInstance();

    void SetListener(ITableListener* listener) { pListener = listener; }

    void Toggle(HWND hWndParent);
    void Update();
    void ExportToCSV(HWND hWndParent);
    void ImportFromCSV(HWND hWndParent); 

    HWND GetHWnd() const { return hDlg; }
};

#endif