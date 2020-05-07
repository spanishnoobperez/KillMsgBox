#pragma once

class CKillMsgBoxDlg : public CDialogEx {
private:
    HMODULE m_hModule;

public:
    CKillMsgBoxDlg(CWnd *pParent = NULL);

    enum { IDD = IDD_KILLMSGBOX_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);

protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnHelpAbout();
    afx_msg void OnBnClickedButtonStart();
};
