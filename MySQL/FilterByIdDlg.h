#pragma once

// CFilterByIdDlg диалоговое окно

class CFilterByIdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterByIdDlg)

public:
	CFilterByIdDlg(CWnd* pParent = nullptr);   // стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTER_BY_ID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CString m_tableName;
	CString m_idValue;
	
	virtual BOOL OnInitDialog();
};
