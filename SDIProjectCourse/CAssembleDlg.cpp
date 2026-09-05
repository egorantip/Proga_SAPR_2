#include "pch.h"
#include "SDIProjectCourse.h"
#include "afxdialogex.h"
#include "CAssembleDlg.h"

IMPLEMENT_DYNAMIC(CAssembleDlg, CDialogEx)

CAssembleDlg::CAssembleDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CAssembleDlg, pParent)
{
}

CAssembleDlg::~CAssembleDlg()
{
}

void CAssembleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PICK, m_comboAssemble);
    // УДАЛЕНО: DDX_CBIndex для m_NumIsp — больше не используется

    DDX_Control(pDX, IDC_EDIT_SALNIK_D, m_editSalnikD);
    DDX_Control(pDX, IDC_EDIT_SALNIK_L, m_editSalnikL);
    DDX_Control(pDX, IDC_EDIT_THREAD_D, m_editThreadD);
    DDX_Control(pDX, IDC_EDIT_THREAD_P, m_editThreadP);
    DDX_Control(pDX, IDC_EDIT_GAYKA_SMALL_D, m_editGaykaSmallD);
    DDX_Control(pDX, IDC_EDIT_GAYKA_L, m_editGaykaL);
    DDX_Control(pDX, IDC_EDIT_GAYKA_D, m_editGaykaD);
    DDX_Control(pDX, IDC_EDIT_GAYKA_S, m_editGaykaS);
    DDX_Control(pDX, IDC_EDIT_SHAYBA_D, m_editShaybaD);
    DDX_Control(pDX, IDC_EDIT_SHAYBA_SMALL_D, m_editShaybaSmallD);
    DDX_Control(pDX, IDC_EDIT_SHAYBA_S, m_editShaybaS);
}

BEGIN_MESSAGE_MAP(CAssembleDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_PICK, &CAssembleDlg::OnCbnSelchangeComboPick)
    ON_BN_CLICKED(IDC_BUTTON_DO, &CAssembleDlg::OnBnClickedButtonDo)
    ON_BN_CLICKED(IDC_APPLY, &CAssembleDlg::OnBnClickedApply)
END_MESSAGE_MAP()

BOOL CAssembleDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_pDoc == nullptr) {
        AfxMessageBox(L"Ошибка: указатель на документ не установлен!");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_comboAssemble.ResetContent();
    for (int i = 1; i <= 6; i++) {
        CString str;
        str.Format(L"%d", i);
        m_comboAssemble.AddString(str);
    }

    int savedIsp = m_pDoc->m_SelectedIsp;
    if (savedIsp >= 1 && savedIsp <= 6) {
        m_comboAssemble.SetCurSel(savedIsp - 1);
        UpdateEdit(savedIsp);
    }
    else {
        m_comboAssemble.SetCurSel(0);
        UpdateEdit(1);
    }

    return TRUE;
}

void CAssembleDlg::OnCbnSelchangeComboPick()
{
    int idx = m_comboAssemble.GetCurSel();
    if (idx != CB_ERR) {
        int var = idx + 1;
        UpdateEdit(var);
    }
}

void CAssembleDlg::OnBnClickedApply()
{
    if (m_pDoc == nullptr) return;

    int idx = m_comboAssemble.GetCurSel();
    if (idx == CB_ERR) return;

    int selectedIsp = idx + 1;
    m_pDoc->m_SelectedIsp = selectedIsp;
    m_pDoc->m_AssemblyType = selectedIsp;
    m_pDoc->SetParams();

    AfxMessageBox(L"Исполнение сборки сохранено!");
}

void CAssembleDlg::OnBnClickedButtonDo()
{
    if (m_pDoc == nullptr) {
        AfxMessageBox(L"Ошибка: документ не задан!");
        return;
    }

    int idx = m_comboAssemble.GetCurSel();
    if (idx != CB_ERR) {
        int var = idx + 1;
        m_pDoc->m_SelectedIsp = var;
        m_pDoc->m_AssemblyType = var;
        m_pDoc->SetParams();
    }

    m_pDoc->LaunchKompas();
    m_pDoc->pKompasApp5 = nullptr;
}

void CAssembleDlg::UpdateEdit(int var)
{
    if (m_pDoc == nullptr) return;

    m_pDoc->m_AssemblyType = var;
    m_pDoc->SetParams();

    CString str;
    str.Format(L"%.1f", m_pDoc->m_Salnik_D);
    m_editSalnikD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Salnik_L);
    m_editSalnikL.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_ThreadD);
    m_editThreadD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_ThreadP);
    m_editThreadP.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Gayka_d);
    m_editGaykaSmallD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Gayka_L);
    m_editGaykaL.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Gayka_D);
    m_editGaykaD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Gayka_S);
    m_editGaykaS.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Shayba_D);
    m_editShaybaD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Shayba_d);
    m_editShaybaSmallD.SetWindowTextW(str);

    str.Format(L"%.1f", m_pDoc->m_Shayba_S);
    m_editShaybaS.SetWindowTextW(str);
}