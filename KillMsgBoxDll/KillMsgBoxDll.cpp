#include "stdafx.h"
#include "WindowsEnum.h"
#include "ApiHook.h"
#include "Judge.h"

#pragma data_seg("MySeg")
HWND g_hWnd = NULL;
BOOL g_bSend = FALSE;
#pragma data_seg()
#pragma comment(linker, "/section:MySeg,RWS")

extern HMODULE g_hModule;
UINT_PTR g_uiTimerId = 0;
HHOOK g_hHook = NULL;

VOID CALLBACK TimerProc(
    HWND hWnd,
    UINT uMsg,
    DWORD dwTime
) {
    EnumUserWindows();
}

LRESULT CALLBACK GetMsgProc(
    int code,
    WPARAM wParam,
    LPARAM lParam
) {

    return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}

extern "C" void SetHook() {

    g_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hModule, 0);
    if (NULL == g_hHook)
        ShowError("SetWindowHookEx");
}

extern "C" void Start() {

    g_uiTimerId = ::SetTimer(NULL, 0, 500, (TIMERPROC)TimerProc);

    SetHook();
}

extern "C" void Stop() {

    if (0 != g_uiTimerId) {
        ::KillTimer(NULL, g_uiTimerId);
        g_uiTimerId = 0;
    }

    if (NULL != g_hHook) {
        if (FALSE == ::UnhookWindowsHookEx(g_hHook))
            ShowError("UnhookWindowsHookEx");
        else
            g_hHook = NULL;
    }
}

