#ifndef _APIHOOK_H_
#define _APIHOOK_H_

#pragma comment(lib, "msimg32.lib")
#include <Wininet.h>

typedef BOOL (__stdcall *typedef_BitBlt)(
    HDC hdcDest,
    int nXDest,
    int nYDest,
    int nWidth,
    int nHeight,
    HDC hdcSrc,
    int nXSrc,
    int nYSrc,
    DWORD dwRop
);

typedef BOOL(__stdcall *typedef_StretchBlt)(
    HDC hdcDest,
    int nXOriginDest,
    int nYOriginDest,
    int nWidthDest,
    int nHeightDest,
    HDC hdcSrc,
    int nXOriginSrc,
    int nYOriginSrc,
    int nWidthSrc,
    int nHeightSrc,
    DWORD dwRop
);

typedef BOOL(__stdcall *typedef_TransparentBlt)(
    HDC hdcDest,
    int nXOriginDest,
    int nYOriginDest,
    int nWidthDest,
    int hHeightDest,
    HDC hdcSrc,
    int nXOriginSrc,
    int nYOriginSrc,
    int nWidthSrc,
    int nHeightSrc,
    UINT crTransparent
);

typedef HINTERNET(__stdcall *typedef_HttpOpenRequestW)(
    HINTERNET hConnect,
    LPCWSTR lpszVerb,
    LPCWSTR lpszObjectName,
    LPCWSTR lpszVersion,
    LPCWSTR lpszReferrer,
    LPCWSTR *lplpszAcceptTypes,
    DWORD dwFlags,
    DWORD dwContext
);

typedef BOOL (__stdcall *typedef_InternetReadFile)(
    IN HINTERNET hFile,
    IN LPVOID lpBuffer,
    IN DWORD dwNumberOfBytesToRead,
    OUT LPDWORD lpdwNumberOfBytesRead
);

BOOL __stdcall New_BitBlt(
    HDC hdcDest,
    int nXDest,
    int nYDest,
    int nWidth,
    int nHeight,
    HDC hdcSrc,
    int nXSrc,
    int nYSrc,
    DWORD dwRop
);

BOOL __stdcall New_StretchBlt(
    HDC hdcDest,
    int nXOriginDest,
    int nYOriginDest,
    int nWidthDest,
    int nHeightDest,
    HDC hdcSrc,
    int nXOriginSrc,
    int nYOriginSrc,
    int nWidthSrc,
    int nHeightSrc,
    DWORD dwRop
);

BOOL __stdcall New_TransparentBlt(
    HDC hdcDest,
    int nXOriginDest,
    int nYOriginDest,
    int nWidthDest,
    int hHeightDest,
    HDC hdcSrc,
    int nXOriginSrc,
    int nYOriginSrc,
    int nWidthSrc,
    int nHeightSrc,
    UINT crTransparent
);

HINTERNET __stdcall New_HttpOpenRequestW(
    HINTERNET hConnect,
    LPCWSTR lpszVerb,
    LPCWSTR lpszObjectName,
    LPCWSTR lpszVersion,
    LPCWSTR lpszReferrer,
    LPCWSTR *lplpszAcceptTypes,
    DWORD dwFlags,
    DWORD dwContext
);

BOOL __stdcall New_InternetReadFile(
    IN HINTERNET hFile,
    IN LPVOID lpBuffer,
    IN DWORD dwNumberOfBytesToRead,
    OUT LPDWORD lpdwNumberOfBytesRead
);

void HookApi();
void HookApi(LPVOID lpOldFunc, LPVOID lpNewFunc, LPVOID lpData, DWORD dwSize);
void UnhookApi();
void UnhookApi(LPVOID lpOldFunc, LPVOID lpData, DWORD dwSize);

#endif