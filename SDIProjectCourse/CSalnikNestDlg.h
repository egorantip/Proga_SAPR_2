#pragma once
#include "afxdialogex.h"
#include "SDIProjectCourseDoc.h"

// Диалоговое окно CSalnikNestDlg

class CSalnikNestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSalnikNestDlg)

public:
	CSalnikNestDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CSalnikNestDlg();
	CSDIProjectCourseDoc* m_pDoc;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSalnikNestDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboSalnikNest;
	virtual BOOL OnInitDialog();
	CEdit m_editSalnikD;
	CEdit m_editSalnikL;
	CEdit m_editThreadD;
	CEdit m_editThreadP;
	void UpdateEdit(int var);
	int m_AssembleType;
	afx_msg void OnBnClickedButtonDo2();
	afx_msg void OnCbnSelchangeComboPick2();
	afx_msg void OnBnClickedApply2();
};
