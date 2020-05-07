#include "stdafx.h"
#include "ApiHook.h"
#include "Judge.h"
#include <Psapi.h>

typedef_BitBlt Old_BitBlt = NULL;
typedef_StretchBlt Old_StretchBlt = NULL;
typedef_TransparentBlt Old_TransparentBlt = NULL;

typedef_HttpOpenRequestW Old_HttpOpenRequestW = NULL;
typedef_InternetReadFile Old_InternetReadFile = NULL;

BYTE m_BitBltData[5] = {0};
BYTE m_StretchBltData[5] = {0};
BYTE m_TransparentBltData[5] = {0};

BYTE m_HttpOpenRequestWData[5] = { 0 };
BYTE m_InternetReadFileData[5] = {0};

void HookApi() {

    HMODULE hModule = ::GetModuleHandle("GDI32.dll");
    if (NULL != hModule) {
        Old_BitBlt = (typedef_BitBlt)::GetProcAddress(hModule, "BitBlt");
        Old_StretchBlt = (typedef_StretchBlt)::GetProcAddress(hModule, "StretchBlt");
    }

    hModule = ::GetModuleHandle("msimg32.dll");
    if (NULL != hModule)
        Old_TransparentBlt = (typedef_TransparentBlt)::GetProcAddress(hModule, "TransparentBlt");

    hModule = ::GetModuleHandle("wininet.dll");
    if (NULL != hModule)
        Old_HttpOpenRequestW = (typedef_HttpOpenRequestW)::GetProcAddress(hModule, "HttpOpenRequestW");

    hModule = ::GetModuleHandle("wininet.dll");
    if (NULL != hModule)
        Old_InternetReadFile = (typedef_InternetReadFile)::GetProcAddress(hModule, "InternetReadFile");

    HookApi(Old_BitBlt, New_BitBlt, m_BitBltData, 5);
    HookApi(Old_StretchBlt, New_StretchBlt, m_StretchBltData, 5);
    HookApi(Old_TransparentBlt, New_TransparentBlt, m_TransparentBltData, 5);
    HookApi(Old_HttpOpenRequestW, New_HttpOpenRequestW, m_HttpOpenRequestWData, 5);
    HookApi(Old_InternetReadFile, New_InternetReadFile, m_InternetReadFileData, 5);

}

void HookApi(LPVOID lpOldFunc, LPVOID lpNewFunc, LPVOID lpData, DWORD dwSize) {
    if ((NULL == lpOldFunc) ||
            (NULL == lpNewFunc))
        return;

    DWORD dwOffset = (DWORD)lpNewFunc - ((DWORD)lpOldFunc + 5);
    BYTE lpOpCode[5] = {0};
    lpOpCode[0] = 0xE9;
    ::RtlCopyMemory(&lpOpCode[1], &dwOffset, sizeof(DWORD));

    DWORD dwOldProtection = 0;
    if (FALSE == ::VirtualProtect(lpOldFunc, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtection))
        ShowError("VirtualProtect");

    ::RtlCopyMemory(lpData, lpOldFunc, 5);

    ::RtlCopyMemory(lpOldFunc, lpOpCode, dwSize);

    if(FALSE == ::VirtualProtect(lpOldFunc, dwSize, dwOldProtection, &dwOldProtection))
        ShowError("VirtualProtect");
}

void UnhookApi() {
    UnhookApi(Old_BitBlt, m_BitBltData, 5);
    UnhookApi(Old_StretchBlt, m_StretchBltData, 5);
    UnhookApi(Old_TransparentBlt, m_TransparentBltData, 5);
    UnhookApi(Old_HttpOpenRequestW, m_HttpOpenRequestWData, 5);
    UnhookApi(Old_InternetReadFile, m_InternetReadFileData, 5);

}

void UnhookApi(LPVOID lpOldFunc, LPVOID lpData, DWORD dwSize) {
    if (NULL == lpOldFunc)
        return;
    BYTE *lpOpCode = (BYTE *)lpData;

    DWORD dwOldProtection = 0;
    if(FALSE == ::VirtualProtect(lpOldFunc, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtection))
        ShowError("VirtualProtect");

    ::RtlCopyMemory(lpOldFunc, lpOpCode, dwSize);

    if(FALSE == ::VirtualProtect(lpOldFunc, dwSize, dwOldProtection, &dwOldProtection))
        ShowError("VirtualProtect");
}

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
) {
    RECT rcRect = {0};
    rcRect.left = nXDest;
    rcRect.top = nYDest;
    rcRect.right = rcRect.left + nWidth;
    rcRect.bottom = rcRect.top + nHeight;
    if (IsMsgBox(rcRect, hdcDest, NULL)) {

        nXDest = 0;
        nYDest = 0;
        nWidth = 0;
        nHeight = 0;
    }

    UnhookApi();

    BOOL bRet = Old_BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);

    HookApi();
    return bRet;
}

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
) {
    RECT rcRect = { 0 };
    rcRect.left = nXOriginDest;
    rcRect.top = nYOriginDest;
    rcRect.right = rcRect.left + nWidthDest;
    rcRect.bottom = rcRect.top + nHeightDest;
    if (IsMsgBox(rcRect, hdcDest, NULL)) {

        nXOriginDest = 0;
        nYOriginDest = 0;
        nWidthDest = 0;
        nHeightDest = 0;
    }

    UnhookApi();

    BOOL bRet = Old_StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
                               hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, dwRop);

    HookApi();
    return bRet;
}

BOOL __stdcall New_TransparentBlt(
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
    UINT crTransparent
) {
    RECT rcRect = { 0 };
    rcRect.left = nXOriginDest;
    rcRect.top = nYOriginDest;
    rcRect.right = rcRect.left + nWidthDest;
    rcRect.bottom = rcRect.top + nHeightDest;
    if (IsMsgBox(rcRect, hdcDest, NULL)) {

        nXOriginDest = 0;
        nYOriginDest = 0;
        nWidthDest = 0;
        nHeightDest = 0;
    }

    UnhookApi();

    BOOL bRet = Old_TransparentBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
                                   hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, crTransparent);

    HookApi();
    return bRet;
}

HINTERNET __stdcall New_HttpOpenRequestW(
    HINTERNET hConnect,
    LPCWSTR lpszVerb,
    LPCWSTR lpszObjectName,
    LPCWSTR lpszVersion,
    LPCWSTR lpszReferrer,
    LPCWSTR *lplpszAcceptTypes,
    DWORD dwFlags,
    DWORD dwContext
) {
    return NULL;

    UnhookApi();

    HINTERNET hInternet = Old_HttpOpenRequestW(hConnect, lpszVerb, lpszObjectName,
                          lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);

    HookApi();
    return hInternet;
}

BOOL __stdcall New_InternetReadFile(
    IN HINTERNET hFile,
    IN LPVOID lpBuffer,
    IN DWORD dwNumberOfBytesToRead,
    OUT LPDWORD lpdwNumberOfBytesRead
) {
    return NULL;

    UnhookApi();

    BOOL bRet = Old_InternetReadFile(hFile, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);

    HookApi();
    return bRet;
}

