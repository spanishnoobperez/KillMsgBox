#include "stdafx.h"
#include "KillMsgBox.h"
#include "KillMsgBoxDlg.h"
#include "afxdialogex.h"
#pragma comment(lib,"KillMsgBoxDll.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx {
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CKillMsgBoxDlg::OnHelpAbout() {

    CAboutDlg dlg;
    dlg.DoModal();
}

CKillMsgBoxDlg::CKillMsgBoxDlg(CWnd *pParent )
    : CDialogEx(CKillMsgBoxDlg::IDD, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKillMsgBoxDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKillMsgBoxDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_HELP_ABOUT, &CKillMsgBoxDlg::OnHelpAbout)
    ON_BN_CLICKED(IDC_BUTTON_START, &CKillMsgBoxDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()

BOOL EnablePriv() {
    HANDLE hToken;
    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
        TOKEN_PRIVILEGES tkp;
        ::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);
        return ((::GetLastError() == ERROR_SUCCESS));
    }
    return FALSE;
}

BOOL CKillMsgBoxDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    EnablePriv();
    return TRUE;
}

void CKillMsgBoxDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else
        CDialogEx::OnSysCommand(nID, lParam);
}

void CKillMsgBoxDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    } else
        CDialogEx::OnPaint();
}

HCURSOR CKillMsgBoxDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

extern "C" void Start();
extern "C" void Stop();

void CKillMsgBoxDlg::OnBnClickedButtonStart() {

    char szText[MAX_PATH] = {0};
    HWND hWnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_START);
    ::GetWindowText(hWnd, szText, MAX_PATH);
    if (0 == ::lstrcmpi(szText, "Start")) {
        Start();
        ::SetWindowText(hWnd, "Stop");
    } else {
        Stop();
        ::SetWindowText(hWnd, "Start");
    }
}
