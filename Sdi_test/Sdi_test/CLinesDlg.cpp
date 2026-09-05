// CLinesDlg.cpp: файл реализации
//

#include "pch.h"
#include "Sdi_test.h"
#include "afxdialogex.h"
#include "CLinesDlg.h"


// Диалоговое окно CLinesDlg

IMPLEMENT_DYNAMIC(CLinesDlg, CDialog)

CLinesDlg::CLinesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LINES_DLG, pParent)
	, m_bCheckRoot(FALSE)
	, m_bCheckCoord(FALSE)
	, m_bCheckHatch(FALSE)
	, m_bCheckSinus(FALSE)
{

}

CLinesDlg::~CLinesDlg()
{
}

void CLinesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ROOT, m_bCheckRoot);
	DDX_Check(pDX, IDC_CHECK_COORD, m_bCheckCoord);
	DDX_Check(pDX, IDC_CHECK_HATCH, m_bCheckHatch);
	DDX_Check(pDX, IDC_CHECK_SINUS, m_bCheckSinus);
}


BEGIN_MESSAGE_MAP(CLinesDlg, CDialog)
    ON_BN_CLICKED(IDC_CHECK_ROOT, &CLinesDlg::OnCheckboxClicked)
    ON_BN_CLICKED(IDC_CHECK_COORD, &CLinesDlg::OnCheckboxClicked)
    ON_BN_CLICKED(IDC_CHECK_HATCH, &CLinesDlg::OnCheckboxClicked)
    ON_BN_CLICKED(IDC_CHECK_SINUS, &CLinesDlg::OnCheckboxClicked)
END_MESSAGE_MAP()


void CLinesDlg::OnCheckboxClicked()
{
    UpdateData(TRUE);

    // Получаем ID нажатого элемента
    UINT nID = GetCurrentMessage()->wParam;

    if (nID == IDC_CHECK_ROOT)
    {
        // Если нажали на "График"
        if (m_bCheckRoot)
        {
            m_bCheckCoord = TRUE;
            m_bCheckHatch = TRUE;
            m_bCheckSinus = TRUE;
        }
        else
        {
            m_bCheckCoord = FALSE;
            m_bCheckHatch = FALSE;
            m_bCheckSinus = FALSE;
        }
    }
    else
    {
        // Если нажали на любой другой чекбокс
        // "График" включен только если все остальные включены
        m_bCheckRoot = (m_bCheckCoord && m_bCheckHatch && m_bCheckSinus);
    }

    UpdateData(FALSE);
}