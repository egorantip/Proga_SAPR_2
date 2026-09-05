// CColorDlg.cpp: файл реализации
//

#include "pch.h"
#include "Sdi_test.h"
#include "afxdialogex.h"
#include "CColorDlg.h"
#include "Sdi_testView.h"


// Диалоговое окно CColorDlg

IMPLEMENT_DYNAMIC(CColorDlg, CDialog)

CColorDlg::CColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COLOR_DLG, pParent)
{

}

CColorDlg::~CColorDlg()
{
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ColorCtrl);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &CColorDlg::OnBnClickedApply)
END_MESSAGE_MAP()


// Обработчики сообщений CColorDlg


void CColorDlg::OnBnClickedApply()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_pView->m_Color = m_ColorCtrl.GetColor();

	m_pView->Invalidate();
}
