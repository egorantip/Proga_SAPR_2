#pragma once

// CConnectDbDlg диалоговое окно

class CConnectDbDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectDbDlg)

public:
	CConnectDbDlg(CWnd* pParent = nullptr);   // стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT_DB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CString m_host;
	int m_port;
	CString m_user;
	CString m_password;
	CString m_database;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

