// FilterByIdDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MySQL.h"
#include "FilterByIdDlg.h"

// CFilterByIdDlg диалоговое окно

IMPLEMENT_DYNAMIC(CFilterByIdDlg, CDialogEx)

CFilterByIdDlg::CFilterByIdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTER_BY_ID, pParent)
	, m_tableName(_T(""))
	, m_idValue(_T(""))
{
}

void CFilterByIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TABLE_NAME, m_tableName);
	DDX_Text(pDX, IDC_ID_VALUE, m_idValue);
}

BEGIN_MESSAGE_MAP(CFilterByIdDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CFilterByIdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

