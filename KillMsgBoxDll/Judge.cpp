#include "stdafx.h"
#include "Judge.h"
#include <stdio.h>
#include <Psapi.h>

void ShowError(char *lpText) {
    char szErr[MAX_PATH] = { 0 };
    ::wsprintf(szErr, "%s Error!\nError Code Is:%d", lpText, ::GetLastError());
#ifdef _DEBUG
    ::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}

void AddInfo(RECT rcPosition, HDC hDC) {
    FILE *fp = NULL;
    fopen_s(&fp, "info.txt", "a+");
    if (NULL != fp) {
        char szInfo[MAX_PATH] = {0};
        if (NULL == hDC)
            ::wsprintf(szInfo, "(%d, %d, %d, %d)\n", rcPosition.left, rcPosition.top, rcPosition.right, rcPosition.bottom);
        else
            ::wsprintf(szInfo, "[HDC](%d, %d, %d, %d)\n", rcPosition.left, rcPosition.top, rcPosition.right, rcPosition.bottom);
        fwrite(szInfo, (1 + ::lstrlen(szInfo)), 1, fp);
        fclose(fp);
    }
}

void AddInfo(char *lpszFileName, char *lpText) {
    FILE *fp = NULL;
    fopen_s(&fp, lpszFileName, "a+");
    if (NULL != fp) {
        fwrite(lpText, (1 + ::lstrlen(lpText)), 1, fp);
        fclose(fp);
    }
}

void AddInfo(RECT rcRect, HWND hWnd, char *lpszInfo) {

    DWORD dwPID = 0;
    ::GetWindowThreadProcessId(hWnd, &dwPID);
    char szFileName[MAX_PATH] = { 0 };
    char szTitle[MAX_PATH] = {0};
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (NULL != hProcess) {
        ::GetModuleFileNameEx(hProcess, NULL, szFileName, MAX_PATH);
        ::CloseHandle(hProcess);
    }
    RECT rc = { 0 };
    ::GetWindowRect(hWnd, &rc);
    ::GetWindowText(hWnd, szTitle, MAX_PATH);
    char szText1[MAX_PATH] = { 0 };
    char szText2[MAX_PATH] = { 0 };
    char szText3[MAX_PATH] = { 0 };
    char szText4[MAX_PATH] = { 0 };
    char szText5[MAX_PATH] = { 0 };
    char szText6[MAX_PATH] = { 0 };
    char szTemp[MAX_PATH] = "\n";
    ::wsprintf(szText1, "RECT(left=%d, top=%d, right=%d, bottom=%d)\nRECT(width=%d, height=%d)\n",
               rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, (rcRect.right - rcRect.left), (rcRect.bottom - rcRect.top));
    ::wsprintf(szText2, "WIN_RECT(left=%d, top=%d, right=%d, bottom=%d)\nWIN_RECT(width=%d, height=%d)\n",
               rc.left, rc.top, rc.right, rc.bottom, (rc.right - rc.left), (rc.bottom - rc.top));
    ::wsprintf(szText3, "PID=%d\n", dwPID);
    ::wsprintf(szText4, "Visible: %d\n", ::IsWindowVisible(hWnd));
    ::wsprintf(szText5, "Title:%s\n", szTitle);

    FILE *fp = NULL;
    fopen_s(&fp, "C:\\Users\\DemonGan\\Desktop\\KillMsgBox\\Debug\\info.txt", "a+");
    if (NULL != fp) {
        fwrite(lpszInfo, (1 + ::lstrlen(lpszInfo)), 1, fp);
        fwrite(szTemp, (1 + ::lstrlen(szTemp)), 1, fp);
        fwrite(szText3, (1 + ::lstrlen(szText3)), 1, fp);
        fwrite(szFileName, (1 + ::lstrlen(szFileName)), 1, fp);
        fwrite(szTemp, (1 + ::lstrlen(szTemp)), 1, fp);
        fwrite(szText4, (1 + ::lstrlen(szText4)), 1, fp);
        fwrite(szText5, (1 + ::lstrlen(szText5)), 1, fp);
        fwrite(szText1, (1 + ::lstrlen(szText1)), 1, fp);
        fwrite(szText2, (1 + ::lstrlen(szText2)), 1, fp);
        fwrite(szTemp, (1 + ::lstrlen(szTemp)), 1, fp);
        fclose(fp);
    }
}

BOOL IsMsgBox(RECT rcPosition, HDC hDC, HWND hWnd) {
    BOOL bRet = FALSE;
    if (NULL != hDC)
        bRet = DCJudge(rcPosition, hDC);
    if (NULL != hWnd)
        bRet = WindowJudge(rcPosition, hWnd);
    return bRet;
}

BOOL WindowJudge(RECT rcPosition, HWND hWnd) {

    int VALID_DISTANCE = 40;
    int VALID_WIDTH = 500;
    int VALID_HEIGHT = 800;
    int VALID_MIN_WIDTH = 80;
    int VALID_MIN_HEIGHT = 80;

    int iBoundWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
    int iBoundHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

    if (((-VALID_DISTANCE <= rcPosition.left) && (rcPosition.left <= VALID_DISTANCE)) ||
            ((iBoundWidth - VALID_DISTANCE <= rcPosition.right) && (rcPosition.right <= iBoundWidth + VALID_DISTANCE)) ||
            ((-VALID_DISTANCE <= rcPosition.top) && (rcPosition.top <= VALID_DISTANCE)) ||
            ((iBoundHeight - VALID_DISTANCE <= rcPosition.bottom) && (rcPosition.bottom <= iBoundHeight + VALID_DISTANCE))) {

        int iWinWidth = rcPosition.right - rcPosition.left;
        int iWinHeight = rcPosition.bottom - rcPosition.top;
        if (((VALID_MIN_WIDTH < iWinWidth) && (iWinWidth <= VALID_WIDTH)) &&
                ((VALID_MIN_HEIGHT < iWinHeight) && (iWinHeight <= VALID_HEIGHT))) {

            if (FALSE == IsGood(hWnd)) {

                AddInfo(rcPosition, hWnd, "Win");

                ::SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                ::SendMessage(hWnd, WM_DESTROY, NULL, NULL);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL DCJudge(RECT rcPosition, HDC hDC) {

    int VALID_DISTANCE = 40;
    int VALID_WIDTH = 500;
    int VALID_HEIGHT = 800;
    int VALID_MIN_WIDTH = 80;
    int VALID_MIN_HEIGHT = 80;

    HWND hWnd = ::WindowFromDC(hDC);

    RECT reRect = { 0 };
    ::GetClientRect(hWnd, &reRect);

    int iBoundWidth = reRect.right - reRect.left;
    int iBoundHeight = reRect.bottom - reRect.top;

    if (((-VALID_DISTANCE <= rcPosition.left) && (rcPosition.left <= VALID_DISTANCE)) ||
            ((iBoundWidth - VALID_DISTANCE <= rcPosition.right) && (rcPosition.right <= iBoundWidth + VALID_DISTANCE)) ||
            ((-VALID_DISTANCE <= rcPosition.top) && (rcPosition.top <= VALID_DISTANCE)) ||
            ((iBoundHeight - VALID_DISTANCE <= rcPosition.bottom) && (rcPosition.bottom <= iBoundHeight + VALID_DISTANCE))) {

        int iWinWidth = rcPosition.right - rcPosition.left;
        int iWinHeight = rcPosition.bottom - rcPosition.top;
        if (((VALID_MIN_WIDTH < iWinWidth) && (iWinWidth <= VALID_WIDTH)) &&
                ((VALID_MIN_HEIGHT < iWinHeight) && (iWinHeight <= VALID_HEIGHT))) {

            if (FALSE == IsGood(hWnd)) {

                AddInfo(rcPosition, hWnd, "HDC");

                ::SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                ::SendMessage(hWnd, WM_DESTROY, NULL, NULL);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL IsGood(HWND hWnd) {

    DWORD dwPID = 0;
    ::GetWindowThreadProcessId(hWnd, &dwPID);
    char szFileName[MAX_PATH] = { 0 };
    char szSysPath[MAX_PATH] = { 0 };
    char szWinPath[MAX_PATH] = { 0 };
    ::GetSystemDirectory(szSysPath, MAX_PATH);
    ::GetWindowsDirectory(szWinPath, MAX_PATH);
    ::lstrcat(szWinPath, "\\explorer.exe");
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (NULL == hProcess) {
    } else {
        ::GetModuleFileNameEx(hProcess, NULL, szFileName, MAX_PATH);
        ::CloseHandle(hProcess);
    }
    _strupr_s(szFileName);
    _strupr_s(szSysPath);
    if (NULL != ::strstr(szFileName, szSysPath) ||
            NULL != ::strstr(szFileName, szWinPath) ||
            0 == ::lstrcmpi(szFileName, szSysPath) ||
            0 == ::lstrcmpi(szFileName, szWinPath))
        return TRUE;

    RECT rcPosition = { 0 };
    ::GetWindowRect(hWnd, &rcPosition);

    if (::IsWindowVisible(hWnd)) {

        return FALSE;
    }
    return TRUE;
}