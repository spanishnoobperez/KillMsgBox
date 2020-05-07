#include "stdafx.h"
#include "WindowsEnum.h"
#include "Judge.h"
#include <Psapi.h>

BOOL CALLBACK EnumWindowsProc(
    HWND hWnd,
    LPARAM lParam
) {

    RECT rcPosition = { 0 };
    ::GetWindowRect(hWnd, &rcPosition);

    IsMsgBox(rcPosition, NULL, hWnd);
    return TRUE;
}

void EnumUserWindows() {
    ::EnumWindows(EnumWindowsProc, NULL);
}