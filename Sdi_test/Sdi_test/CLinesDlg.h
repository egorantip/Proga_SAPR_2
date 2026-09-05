#pragma once
#include "afxdialogex.h"


// Диалоговое окно CLinesDlg

class CLinesDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinesDlg)

public:
	CLinesDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CLinesDlg();

	BOOL m_bCheckCoord, m_bCheckHatch, m_bCheckSinus, m_bCheckRoot;

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINES_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	afx_msg void OnCheckboxClicked();

	DECLARE_MESSAGE_MAP()
};
