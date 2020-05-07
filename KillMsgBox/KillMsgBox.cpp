#include "stdafx.h"
#include "KillMsgBox.h"
#include "KillMsgBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CKillMsgBoxApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CKillMsgBoxApp::CKillMsgBoxApp() {

    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}

CKillMsgBoxApp theApp;

BOOL CKillMsgBoxApp::InitInstance() {

    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);

    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();
    AfxEnableControlContainer();

    CShellManager *pShellManager = new CShellManager;

    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    SetRegistryKey(_T("Native Application"));
    CKillMsgBoxDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {

    } else if (nResponse == IDCANCEL) {

    } else if (nResponse == -1) {
        TRACE(traceAppMsg, 0, "Failed to create the dialog and the program will exit immediately\n");
    }

    if (pShellManager != NULL)
        delete pShellManager;

    return FALSE;
}

