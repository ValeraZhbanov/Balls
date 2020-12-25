#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <Commctrl.h>
#include <vector>

#include "Settings.h"

#define IDC_PAUSE                       1001
#define IDC_NEW                         1002
#define IDC_CONTINUE                    1003
#define IDC_SETTING                     1004
#define IDC_HELP                        1005

#define IDT_OPENMENU                    1001
#define IDT_CLOSEMENU                   1002
#define IDT_GAME                        1003


#define IDD_SETTING                     101
#define IDC_SET                         1001
#define IDC_CANCEL                      1002
#define IDC_BODYCOLOR1                  1003
#define IDC_SPEED1                      1004
#define IDC_SIZE1                       1005
#define IDC_BULLETCOLOR1                1006
#define IDC_BULLETSPEED1                1007
#define IDC_BULLETSIZE1                 1008
#define IDC_BULLETDISTANCE1             1009
#define IDC_BULLETCHANCE1               1010
#define IDC_COUNT                       1011
#define IDC_BODYCOLOR2                  1019
#define IDC_SPEED2                      1020
#define IDC_SIZE2                       1021
#define IDC_BULLETCOLOR2                1022
#define IDC_BULLETSPEED2                1023
#define IDC_BULLETSIZE2                 1024
#define IDC_BULLETDISTANCE2             1025
#define IDC_BULLETCHANCE2               1026


#define IDD_HELP                        102
#define IDC_TEXT                        1001
#define IDC_CANCEL                      1002

#define IDI_GAME                        101
#define IDM_OPENGAME                    1001
#define IDM_EXIT                        1002
#define WM_SHELLNOTIFY                  (WM_USER + 1)

#define HANDLE_WM_SHELLNOTIFY(hWnd, wParam, lParam, fn) \
    ((fn)((hWnd), (INT)(wParam), (INT)(lParam)), 0L)

auto Icon = (HICON)LoadImage(0, "icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
auto AppName = "Balls";

class ToolTip {
public:
    ToolTip(HWND hWnd, HWND hWndTool, LPCSTR pszText) {
        HWND tooltip = CreateWindowEx(0, TOOLTIPS_CLASS, 0, WS_POPUP | TTS_ALWAYSTIP | TTS_CLOSE | TTS_USEVISUALSTYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, 0, GetModuleHandle(0), 0);

        TOOLINFO ti{};
        ti.cbSize = sizeof(ti);
        ti.hwnd = hWnd;
        ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_PARSELINKS;
        ti.uId = (UINT_PTR)hWndTool;
        ti.lpszText = (LPSTR)pszText;

        SendMessage(tooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        SendMessage(tooltip, TTM_SETMAXTIPWIDTH, 0, 150);
    }
};

class MainMenu {
    HWND Menu = {};
    INT CurrX = {};
public:
    HWND New = {};
    HWND Continue = {};

    HWND LastScore = {};
    HWND LastDeath = {};

    HWND BestScore = {};
    HWND BestDeath = {};

    HWND Setting = {};
    HWND Help = {};

    INT Width = 800;

    MainMenu() {}
    MainMenu(HWND hWnd) : CurrX(0) {
        HWND child = {};
        int style = WS_CHILD | WS_VISIBLE;
        Menu = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, "static", "", style | WS_BORDER, CurrX, 0, 400, Width, hWnd, (HMENU)0, GetModuleHandle(0), 0);

        New = CreateWindowEx(0, "button", "Новая игра", style | WS_BORDER | BS_OWNERDRAW, CurrX + 10, 10, 190, 30, hWnd, (HMENU)IDC_NEW, GetModuleHandle(0), 0);
        ToolTip(hWnd, New, "Запуск игры");
        Continue = CreateWindowEx(0, "button", "Продолжить", style | WS_BORDER | BS_OWNERDRAW, CurrX + 200, 10, 190, 30, hWnd, (HMENU)IDC_CONTINUE, GetModuleHandle(0), 0);
        ToolTip(hWnd, Continue, "Продолжить игру или начать новую");

        child = CreateWindowEx(0, "button", "Прошлая игра", style | BS_GROUPBOX, 10, 50, 375, 90, Menu, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Счёт:", style | SS_LEFT, 10, 30, 100, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        LastScore = CreateWindowEx(0, "static", "", style | WS_BORDER | SS_CENTER, 110, 30, 240, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Смерть:", style | SS_LEFT, 10, 60, 100, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        LastDeath = CreateWindowEx(0, "static", "", style | WS_BORDER | SS_CENTER, 110, 60, 240, 20, child, (HMENU)0, GetModuleHandle(0), 0);

        child = CreateWindowEx(0, "button", "Лучшая игра", style | BS_GROUPBOX, 10, 150, 375, 90, Menu, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Счёт:", style | SS_LEFT, 10, 30, 100, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        BestScore = CreateWindowEx(0, "static", "", style | WS_BORDER | SS_CENTER, 110, 30, 240, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        CreateWindowEx(0, "static", "Смерть:", style | SS_LEFT, 10, 60, 100, 20, child, (HMENU)0, GetModuleHandle(0), 0);
        BestDeath = CreateWindowEx(0, "static", "", style | WS_BORDER | SS_CENTER, 110, 60, 240, 20, child, (HMENU)0, GetModuleHandle(0), 0);

        Setting = CreateWindowEx(0, "button", "", style | BS_BITMAP, CurrX + 10, 760, 30, 30, hWnd, (HMENU)IDC_SETTING, GetModuleHandle(0), 0);
        SendMessage(Setting, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "1.bmp", IMAGE_BITMAP, 26, 26, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        ToolTip(hWnd, Setting, "Настройки");
        Help = CreateWindowEx(0, "button", "", style | BS_BITMAP, CurrX + 44, 760, 30, 30, hWnd, (HMENU)IDC_HELP, GetModuleHandle(0), 0);
        SendMessage(Help, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(0, "2.bmp", IMAGE_BITMAP, 26, 26, LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        ToolTip(hWnd, Help, "Помощь");
    }

    bool OpenTick() {
        if(CurrX < 0) {
            CurrX += 10;
            Paint();
            return false;
        }
        return true;
    }

    bool CloseTick() {
        if(-400 < CurrX) {
            CurrX -= 10;
            Paint();
            return false;
        }
        return true;
    }

    void Paint() {
        MoveWindow(New, CurrX + 10, 10, 190, 30, 0);
        MoveWindow(Continue, CurrX + 200, 10, 190, 30, 0);
        MoveWindow(Setting, CurrX + 10, Width - 40, 30, 30, 0);
        MoveWindow(Help, CurrX + 44, Width - 40, 30, 30, 0);
        MoveWindow(Menu, CurrX, 0, 400, Width, 1);
    }
};


class Main {
    HDC hDC = {};
    HDC hDCM = {};
    HWND hWnd = {};

public:
    RECT RT = {};
    MainMenu Menu = {};
    HMENU ContextMenu = CreateContextMenu();

    Main() {}
    Main(WNDPROC wndproc) {

        WNDCLASS wc{};
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
        wc.lpfnWndProc = wndproc;
        wc.cbClsExtra;
        wc.cbWndExtra;
        wc.hInstance = GetModuleHandle(0);
        wc.hIcon = Icon;
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground;
        wc.lpszMenuName;
        wc.lpszClassName = "Window";
        RegisterClass(&wc);

        hWnd = CreateWindowEx(0, "Window", AppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 1400, 840, 0, 0, GetModuleHandle(0), 0);
        Center(hWnd);
        Size();
        hDC = GetDC(hWnd);
        hDCM = CreateCompatibleDC(hDC);
        SetBkMode(hDCM, TRANSPARENT);
        SelectObject(hDCM, CreateCompatibleBitmap(hDC, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
        SelectObject(hDCM, GetStockObject(NULL_PEN));
        Fill();
        Menu = MainMenu(hWnd);
    }

    void Fill() {
        FillRect(hDCM, &RT, GameSetting.Battlefield.Color);
    }

    void Size() {
        GetClientRect(hWnd, &RT);
        Fill();

        Menu.Width = RT.bottom;
        Menu.Paint();
    }

    void Paint() {
        BitBlt(hDC, RT.left, RT.top, RT.right, RT.bottom, hDCM, 0, 0, SRCCOPY);
    }

    POINT GetRndPoint() {
        return {(LONG)rnd(RT.left, RT.right), (LONG)rnd(RT.top, RT.bottom)};
    }

    operator HDC() {
        return hDCM;
    }

    operator HWND() {
        return hWnd;
    }

    operator BOOL() {
        return IsWindow(hWnd);
    }

private:

    HMENU CreateContextMenu() {
        auto hMainMenu = CreatePopupMenu();
        AppendMenu(hMainMenu, MF_STRING, IDC_PAUSE, "Пауза");
        AppendMenu(hMainMenu, MF_STRING, IDC_SETTING, "Настройки");
        AppendMenu(hMainMenu, MF_STRING, IDC_HELP, "Помощь");
        return hMainMenu;
    }

    void Center(HWND hWnd) {
        RECT RT;
        GetWindowRect(hWnd, &RT);
        auto width = RT.right - RT.left;
        auto height = RT.bottom - RT.top;
        auto x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        auto y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
        MoveWindow(hWnd, x, y, width, height, true);
    }

} Window;

class ChooseColorDialog {
    CHOOSECOLOR cc{};
    COLORREF acrCustClr[16]{};

public:
    ChooseColorDialog(HWND hWnd) {
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hWnd;
        cc.lpCustColors = acrCustClr;
    }

    INT Show() {
        ChooseColor(&cc);
        return cc.rgbResult;
    }
};

class ItemDraw {
    HWND hWnd;
    const DRAWITEMSTRUCT * lpDrawItem = {};

    void DrawButton() {
        auto hDCM = CreateCompatibleDC(lpDrawItem->hDC);
        auto bitmap = SelectObject(hDCM, CreateCompatibleBitmap(lpDrawItem->hDC, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom));
        auto brush = SelectObject(hDCM, CreateSolidBrush(0xAAAAAA));
        auto font = SelectObject(hDCM, SelectObject(lpDrawItem->hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)));

        char text[30]{};
        GetWindowText(lpDrawItem->hwndItem, text, 30);

        SetBkMode(hDCM, TRANSPARENT);
        SetTextColor(hDCM, 0x222222);

        switch(lpDrawItem->itemAction) {
            case ODA_FOCUS:
                if(lpDrawItem->itemState & ODS_FOCUS)
                    DeleteBrush(SelectObject(hDCM, CreateSolidBrush(0x00ff00)));
                break;
            case ODA_SELECT:
                if(lpDrawItem->itemState & ODS_SELECTED) {
                    DeleteBrush(SelectObject(hDCM, CreateSolidBrush(0x222222)));
                    SetTextColor(hDCM, 0xEEEEEE);
                }
                break;
        }
        Rectangle(hDCM, lpDrawItem->rcItem.left, lpDrawItem->rcItem.top, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom);
        DrawText(hDCM, text, strlen(text), (LPRECT)&lpDrawItem->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        BitBlt(lpDrawItem->hDC, 0, 0, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom, hDCM, 0, 0, SRCCOPY);

        DeleteObject(SelectObject(lpDrawItem->hDC, SelectObject(hDCM, font)));
        DeleteObject(SelectObject(hDCM, bitmap));
        DeleteObject(SelectObject(hDCM, brush));
        DeleteDC(hDCM);
    }

public:
    ItemDraw(const DRAWITEMSTRUCT * lpDrawItem = 0) : lpDrawItem(lpDrawItem) {
        if(lpDrawItem)
            switch(lpDrawItem->CtlType) {
                case ODT_BUTTON: DrawButton(); return;
            }
    }
};