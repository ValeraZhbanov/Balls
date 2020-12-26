#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 

#include <Windows.h>
#include <windowsx.h>
#include <Commctrl.h>

#include <fstream>
#include <string>
#include <regex>
#include <locale>
#include <codecvt>

#include "Gui.h"
#include "Game.h"

BOOL WINAPI DlgSettingProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    static HWND color1 = {};
    static HWND speed1 = {};
    static HWND size1 = {};
    static HWND bulletcolor1 = {};
    static HWND bulletspeed1 = {};
    static HWND bulletsize1 = {};
    static HWND bulletdistance1 = {};
    static HWND bulletchance1 = {};

    static HWND count2 = {};
    static HWND color2 = {};
    static HWND speed2 = {};
    static HWND size2 = {};
    static HWND bulletcolor2 = {};
    static HWND bulletspeed2 = {};
    static HWND bulletsize2 = {};
    static HWND bulletdistance2 = {};
    static HWND bulletchance2 = {};

    static Setting setting;

    auto Cls_OnInitDialog = [](HWND hWnd, HWND hWndFocus, LPARAM lParam) {
        EnableWindow(Window, 0);

        setting = GameSetting;

        color1 = GetDlgItem(hWnd, IDC_BODYCOLOR1);
        ToolTip(hWnd, color1, "Изменить цвет");
        speed1 = GetDlgItem(hWnd, IDC_SPEED1);
        SendMessage(speed1, TBM_SETRANGE, TRUE, MAKELPARAM(1, 25));
        SendMessage(speed1, TBM_SETPOS, TRUE, setting.Player.Speed.Max);
        size1 = GetDlgItem(hWnd, IDC_SIZE1);
        SendMessage(size1, TBM_SETRANGE, TRUE, MAKELPARAM(10, 50));
        SendMessage(size1, TBM_SETPOS, TRUE, setting.Player.Size);
        bulletcolor1 = GetDlgItem(hWnd, IDC_BULLETCOLOR1);
        ToolTip(hWnd, bulletcolor1, "Изменить цвет");
        bulletspeed1 = GetDlgItem(hWnd, IDC_BULLETSPEED1);
        SendMessage(bulletspeed1, TBM_SETRANGE, TRUE, MAKELPARAM(1, 25));
        SendMessage(bulletspeed1, TBM_SETPOS, TRUE, setting.Player.Bullet.Speed);
        bulletsize1 = GetDlgItem(hWnd, IDC_BULLETSIZE1);
        SendMessage(bulletsize1, TBM_SETRANGE, TRUE, MAKELPARAM(1, 100));
        SendMessage(bulletsize1, TBM_SETPOS, TRUE, setting.Player.Bullet.Size);
        bulletdistance1 = GetDlgItem(hWnd, IDC_BULLETDISTANCE1);
        SendMessage(bulletdistance1, CB_ADDSTRING, 0, (LPARAM)"Малая");
        SendMessage(bulletdistance1, CB_ADDSTRING, 0, (LPARAM)"Средняя");
        SendMessage(bulletdistance1, CB_ADDSTRING, 0, (LPARAM)"Большая");
        SendMessage(bulletdistance1, CB_SETCURSEL, (setting.Player.Bullet.Time - 50) / 400, 0);
        bulletchance1 = GetDlgItem(hWnd, IDC_BULLETCHANCE1);
        SendMessage(bulletchance1, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
        SendMessage(bulletchance1, TBM_SETPOS, TRUE, setting.Player.Bullet.ChanceShoot * 10);

        count2 = GetDlgItem(hWnd, IDC_COUNT);
        SendMessage(count2, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));
        SendMessage(count2, TBM_SETPOS, TRUE, setting.Enemy.Count);
        color2 = GetDlgItem(hWnd, IDC_BODYCOLOR2);
        ToolTip(hWnd, color2, "Изменить цвет");
        speed2 = GetDlgItem(hWnd, IDC_SPEED2);
        SendMessage(speed2, TBM_SETRANGE, TRUE, MAKELPARAM(1, 25));
        SendMessage(speed2, TBM_SETPOS, TRUE, setting.Enemy.Speed.Max);
        size2 = GetDlgItem(hWnd, IDC_SIZE2);
        SendMessage(size2, TBM_SETRANGE, TRUE, MAKELPARAM(10, 50));
        SendMessage(size2, TBM_SETPOS, TRUE, setting.Enemy.Size);
        bulletcolor2 = GetDlgItem(hWnd, IDC_BULLETCOLOR2);
        ToolTip(hWnd, bulletcolor2, "Изменить цвет");
        bulletspeed2 = GetDlgItem(hWnd, IDC_BULLETSPEED2);
        SendMessage(bulletspeed2, TBM_SETRANGE, TRUE, MAKELPARAM(1, 25));
        SendMessage(bulletspeed2, TBM_SETPOS, TRUE, setting.Enemy.Bullet.Speed);
        bulletsize2 = GetDlgItem(hWnd, IDC_BULLETSIZE2);
        SendMessage(bulletsize2, TBM_SETRANGE, TRUE, MAKELPARAM(1, 100));
        SendMessage(bulletsize2, TBM_SETPOS, TRUE, setting.Enemy.Bullet.Size);
        bulletdistance2 = GetDlgItem(hWnd, IDC_BULLETDISTANCE2);
        SendMessage(bulletdistance2, CB_ADDSTRING, 0, (LPARAM)"Малая");
        SendMessage(bulletdistance2, CB_ADDSTRING, 0, (LPARAM)"Средняя");
        SendMessage(bulletdistance2, CB_ADDSTRING, 0, (LPARAM)"Большая");
        SendMessage(bulletdistance2, CB_SETCURSEL, (setting.Enemy.Bullet.Time - 50) / 400, 0);
        bulletchance2 = GetDlgItem(hWnd, IDC_BULLETCHANCE2);
        SendMessage(bulletchance2, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
        SendMessage(bulletchance2, TBM_SETPOS, TRUE, setting.Enemy.Bullet.ChanceShoot * 10);

        ToolTip(hWnd, GetDlgItem(hWnd, IDC_SET), "Установить настройки");
        ToolTip(hWnd, GetDlgItem(hWnd, IDC_CANCEL), "Закрыть");

        return TRUE;
    };

    auto Cls_OnClose = [](HWND hWnd) {
        EnableWindow(Window, 1);
        DestroyWindow(hWnd);
    };

    auto Cls_OnCommand = [](HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify) {
        switch(id) {
            case IDC_BODYCOLOR1:
                if(auto color = ChooseColorDialog(hWnd).Show()) {
                    DeleteBrush(setting.Player.BodyColor);
                    setting.Player.BodyColor = CreateSolidBrush(color);
                    InvalidateRect(hWndCtl, 0, 0);
                }
                return;
            case IDC_BULLETCOLOR1:
                if(auto color = ChooseColorDialog(hWnd).Show()) {
                    DeleteBrush(setting.Player.Bullet.Color);
                    setting.Player.Bullet.Color = CreateSolidBrush(color);
                    InvalidateRect(hWndCtl, 0, 0);
                }
                return;
            case IDC_BODYCOLOR2:
                if(auto color = ChooseColorDialog(hWnd).Show()) {
                    DeleteBrush(setting.Enemy.BodyColor);
                    setting.Enemy.BodyColor = CreateSolidBrush(color);
                    InvalidateRect(hWndCtl, 0, 0);
                }
                return;
            case IDC_BULLETCOLOR2:
                if(auto color = ChooseColorDialog(hWnd).Show()) {
                    DeleteBrush(setting.Enemy.Bullet.Color);
                    setting.Enemy.Bullet.Color = CreateSolidBrush(color);
                    InvalidateRect(hWndCtl, 0, 0);
                }
                return;
            case IDC_SET:
                setting.Player.Speed.Max = SendMessage(speed1, TBM_GETPOS, 0, 0);
                setting.Player.Size = SendMessage(size1, TBM_GETPOS, 0, 0);
                setting.Player.Bullet.Speed = SendMessage(bulletspeed1, TBM_GETPOS, 0, 0);
                setting.Player.Bullet.Size = SendMessage(bulletsize1, TBM_GETPOS, 0, 0);
                setting.Player.Bullet.ChanceShoot = (DOUBLE)SendMessage(bulletchance1, TBM_GETPOS, 0, 0) / 10;
                setting.Player.Bullet.Time = 50 + SendMessage(bulletdistance1, CB_GETCURSEL, 0, 0) * 400;

                setting.Enemy.Count = SendMessage(count2, TBM_GETPOS, 0, 0);
                setting.Enemy.Speed.Max = SendMessage(speed2, TBM_GETPOS, 0, 0);
                setting.Enemy.Size = SendMessage(size2, TBM_GETPOS, 0, 0);
                setting.Enemy.Bullet.Speed = SendMessage(bulletspeed2, TBM_GETPOS, 0, 0);
                setting.Enemy.Bullet.Size = SendMessage(bulletsize2, TBM_GETPOS, 0, 0);
                setting.Enemy.Bullet.ChanceShoot = (DOUBLE)SendMessage(bulletchance2, TBM_GETPOS, 0, 0) / 10;
                setting.Enemy.Bullet.Time = 50 + SendMessage(bulletdistance2, CB_GETCURSEL, 0, 0) * 400;

                if(MessageBox(hWnd, "Применить выбранные настройки?", "Предупреждение", MB_ICONINFORMATION | MB_YESNO) == IDYES)
                    GameSetting = setting;
            case IDC_CANCEL:
                SendMessage(hWnd, WM_CLOSE, 0, 0);
                return;
        }
    };

    auto Cls_OnDrawItem = [](HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem) {
        if(lpDrawItem->hwndItem == color1) {
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, setting.Player.BodyColor);
        } else if(lpDrawItem->hwndItem == bulletcolor1) {
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, setting.Player.Bullet.Color);
        } else if(lpDrawItem->hwndItem == color2) {
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, setting.Enemy.BodyColor);
        } else if(lpDrawItem->hwndItem == bulletcolor2) {
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, setting.Enemy.Bullet.Color);
        } else ItemDraw draw(lpDrawItem);
    };

    auto Cls_OnSetCursor = [](HWND hWnd, HWND hWndCursor, UINT codeHitTest, UINT msg) {
        SetFocus(hWndCursor);
        return FALSE;
    };

    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Cls_OnInitDialog);
        HANDLE_MSG(hWnd, WM_CLOSE, Cls_OnClose);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_DRAWITEM, Cls_OnDrawItem);
        HANDLE_MSG(hWnd, WM_SETCURSOR, Cls_OnSetCursor);
        default: return 0;
    }
}

BOOL WINAPI DlgAboutProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    static HWND text = {};
    static HWND cancel = {};

    auto Cls_OnInitDialog = [&](HWND hWnd, HWND hWndFocus, LPARAM lParam) {
        EnableWindow(Window, 0);

        text = GetDlgItem(hWnd, IDC_TEXT);
        cancel = GetDlgItem(hWnd, IDC_CANCEL);

        std::wifstream fin(L"about.txt", std::wifstream::binary);
        fin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
        std::wstring str;
        std::getline(fin, str, L'\0');
        SetWindowTextW(text, str.c_str());

        return TRUE;
    };

    auto Cls_OnClose = [](HWND hWnd) {
        EnableWindow(Window, 1);
        DestroyWindow(hWnd);
    };

    auto Cls_OnCommand = [](HWND hWnd, INT id, HWND hwndCtl, UINT codeNotify) {
        switch(id) {
            case IDC_CANCEL:
                SendMessage(hWnd, WM_CLOSE, 0, 0);
                return;
        }
    };

    auto Cls_OnDrawItem = [](HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem) {
        ItemDraw draw(lpDrawItem);
    };

    auto Cls_OnSetCursor = [](HWND hWnd, HWND hWndCursor, UINT codeHitTest, UINT msg) {
        SetFocus(hWndCursor);
        return FALSE;
    };

    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Cls_OnInitDialog);
        HANDLE_MSG(hWnd, WM_CLOSE, Cls_OnClose);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_DRAWITEM, Cls_OnDrawItem);
        HANDLE_MSG(hWnd, WM_SETCURSOR, Cls_OnSetCursor);
        default: return 0;
    }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {

    auto Cls_OnCreate = [&](HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
        return TRUE;
    };

    auto Cls_OnDestroy = [](HWND hWnd) {

    };

    auto Cls_OnMouseMove = [](HWND hWnd, INT x, INT y, UINT keyFlags) {
        CurrentGame.Player.Purpose.x = x;
        CurrentGame.Player.Purpose.y = y;
    };

    auto Cls_OnCommand = [](HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify) {
        switch(id) {
            case IDC_NEW:
                CurrentGame = Game();
                SetTimer(hWnd, IDT_CLOSEMENU, 1, 0);
                SetTimer(hWnd, IDT_GAME, 10, 0);
                return;
            case IDC_PAUSE:
                KillTimer(hWnd, IDT_CLOSEMENU);
                KillTimer(hWnd, IDT_GAME);
                SetTimer(hWnd, IDT_OPENMENU, 10, 0);
                return;
            case IDC_CONTINUE:
                Window.Fill();
                KillTimer(hWnd, IDT_OPENMENU);
                SetTimer(hWnd, IDT_CLOSEMENU, 1, 0);
                SetTimer(hWnd, IDT_GAME, 10, 0);
                return;
            case IDC_SETTING:
                KillTimer(hWnd, IDT_GAME);
                SetTimer(hWnd, IDT_OPENMENU, 10, 0);
                CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETTING), hWnd, DlgSettingProc);
                return;
            case IDC_HELP:
                KillTimer(hWnd, IDT_GAME);
                SetTimer(hWnd, IDT_OPENMENU, 10, 0);
                CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_HELP), hWnd, DlgAboutProc);
                return;
        }
    };

    auto Cls_OnPaint = [](HWND hWnd) {
        PAINTSTRUCT PS;
        BeginPaint(hWnd, &PS);
        Window.Paint();
        EndPaint(hWnd, &PS);
    };

    auto Cls_OnKey = [](HWND hWnd, UINT vk, BOOL fDown, INT cRepeat, UINT flags) {
        switch(vk) {
            case VK_F1:
                SendMessage(hWnd, WM_COMMAND, IDC_HELP, 0);
                break;
            case VK_F2:
                SendMessage(hWnd, WM_COMMAND, IDC_PAUSE, 0);
                break;
        }
    };

    auto Cls_OnContextMenu = [](HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos) {
        TrackPopupMenu(Window.ContextMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hWnd, 0);
    };

    auto Cls_OnTimer = [](HWND hWnd, UINT id) {
        switch(id) {
            case IDT_OPENMENU:
                if(Window.Menu.OpenTick()) {
                    KillTimer(hWnd, IDT_OPENMENU);
                    SetFocus(hWnd);
                }
                return;
            case IDT_CLOSEMENU:
                if(Window.Menu.CloseTick()) {
                    KillTimer(hWnd, IDT_CLOSEMENU);
                    SetFocus(hWnd);
                }
                return;
            case IDT_GAME:
                CurrentGame.Tick();
                return;
        }
    };

    auto Cls_OnDrawItem = [](HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem) {
        ItemDraw draw(lpDrawItem);
    };

    auto Cls_OnSetCursor = [](HWND hWnd, HWND hWndCursor, UINT codeHitTest, UINT msg) {
        SetFocus(hWndCursor);
        return FORWARD_WM_SETCURSOR(hWnd, hWndCursor, codeHitTest, msg, DefWindowProc);
    };

    auto Cls_OnSize = [](HWND hwnd, UINT state, int cx, int cy) {
        Window.Size();
    };

    auto Cls_OnGetMinMaxInfo = [](HWND hwnd, LPMINMAXINFO lpMinMaxInfo) {
        lpMinMaxInfo->ptMinTrackSize = {800, 600};
    };

    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, Cls_OnMouseMove);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
        HANDLE_MSG(hWnd, WM_KEYDOWN, Cls_OnKey);
        HANDLE_MSG(hWnd, WM_CONTEXTMENU, Cls_OnContextMenu);
        HANDLE_MSG(hWnd, WM_TIMER, Cls_OnTimer);
        HANDLE_MSG(hWnd, WM_DRAWITEM, Cls_OnDrawItem);
        HANDLE_MSG(hWnd, WM_SETCURSOR, Cls_OnSetCursor);
        HANDLE_MSG(hWnd, WM_SIZE, Cls_OnSize);
        HANDLE_MSG(hWnd, WM_GETMINMAXINFO, Cls_OnGetMinMaxInfo);
        default: return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
}

LRESULT WINAPI TaskBarProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    static HMENU PopupMenu = {};
    static NOTIFYICONDATA NotifyIcon = {};

    auto Cls_OnCreate = [](HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
        PopupMenu = CreatePopupMenu();
        AppendMenu(PopupMenu, MF_STRING, IDM_OPENGAME, "Вернуться в игру");
        AppendMenu(PopupMenu, MF_STRING, IDM_EXIT, "Закрыть");

        NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
        NotifyIcon.hWnd = hWnd;
        NotifyIcon.uID = IDI_GAME;
        NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_STATE;
        NotifyIcon.uCallbackMessage = WM_SHELLNOTIFY;
        NotifyIcon.hIcon = Icon;

        lstrcpy(NotifyIcon.szTip, AppName);
        ShowWindow(hWnd, SW_HIDE);
        Shell_NotifyIcon(NIM_ADD, &NotifyIcon);

        SendMessage(hWnd, WM_COMMAND, IDM_OPENGAME, 0);
        return BOOL(1);
    };

    auto Cls_OnDestroy = [](HWND hWnd) {
        PostQuitMessage(0);
    };

    auto Cls_OnCommand = [](HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify) {
        switch(id) {
            case IDM_OPENGAME:
                if(!Window) Window = Main(WndProc);
                else ShowWindow(Window, 10);
                return;
            case IDM_EXIT:
                Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
                SendMessage(hWnd, WM_DESTROY, 0, 0);
                return;
        }
    };

    auto Cls_OnShellNotify = [](HWND hWnd, INT id, UINT codeNotify) {
        if(id == IDI_GAME) {
            switch(codeNotify) {
                case WM_RBUTTONDOWN | WM_RBUTTONUP:
                    POINT PT;
                    GetCursorPos(&PT);
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(PopupMenu, TPM_RIGHTALIGN, PT.x, PT.y, 0, hWnd, 0);
                    return;
                case WM_LBUTTONDOWN:
                    SendMessage(hWnd, WM_COMMAND, IDM_OPENGAME, 0);
                    return;
            }
        }
    };

    switch(uMessage) {
        HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
        HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_SHELLNOTIFY, Cls_OnShellNotify);
        default: return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {

    WNDCLASS wc{};
    wc.style;
    wc.lpfnWndProc = TaskBarProc;
    wc.cbClsExtra;
    wc.cbWndExtra;
    wc.hInstance = hInstance;
    wc.hIcon = Icon;
    wc.hCursor;
    wc.hbrBackground;
    wc.lpszMenuName;
    wc.lpszClassName = "TaskBar";
    RegisterClass(&wc);

    CreateWindowEx(WS_EX_LEFT, "TaskBar", "", WS_POPUP, 0, 0, 0, 0, 0, 0, hInstance, 0);

    MSG msg;
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}