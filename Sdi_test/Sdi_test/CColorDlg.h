#pragma once
#include "afxdialogex.h"

class CSditestView;

// Диалоговое окно CColorDlg

class CColorDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CColorDlg();

	CSditestView* m_pView;

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	CMFCColorButton m_ColorCtrl;
};
