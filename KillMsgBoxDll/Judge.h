#ifndef _JUDGE_H_
#define _JUDGE_H_

void ShowError(char *lpText);

void AddInfo(RECT rcPosition, HDC hDC);
void AddInfo(char *lpszFileName, char *lpText);
void AddInfo(RECT rcRect, HWND hWnd, char *lpszInfo);

BOOL IsMsgBox(RECT rcPosition, HDC hDC = NULL, HWND hWnd = NULL);

BOOL WindowJudge(RECT rcPosition, HWND hWnd);

BOOL DCJudge(RECT rcPosition, HDC hDC);

BOOL IsGood(HWND hWnd);

#endif