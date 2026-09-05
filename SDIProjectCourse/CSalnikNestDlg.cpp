#include "pch.h"
#include "SDIProjectCourse.h"
#include "afxdialogex.h"
#include "CSalnikNestDlg.h"

IMPLEMENT_DYNAMIC(CSalnikNestDlg, CDialogEx)

CSalnikNestDlg::CSalnikNestDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CSalnikNestDlg, pParent)
{
}

CSalnikNestDlg::~CSalnikNestDlg()
{
}

void CSalnikNestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PICK2, m_comboSalnikNest);
    DDX_Control(pDX, IDC_EDIT_SALNIK_D_2, m_editSalnikD);
    DDX_Control(pDX, IDC_EDIT_SALNIK_L_2, m_editSalnikL);
    DDX_Control(pDX, IDC_EDIT_THREAD_D_2, m_editThreadD);
    DDX_Control(pDX, IDC_EDIT_THREAD_P_2, m_editThreadP);
}

BEGIN_MESSAGE_MAP(CSalnikNestDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_PICK2, &CSalnikNestDlg::OnCbnSelchangeComboPick2)
    ON_BN_CLICKED(IDC_BUTTON_DO2, &CSalnikNestDlg::OnBnClickedButtonDo2)
    ON_BN_CLICKED(IDC_APPLY_2, &CSalnikNestDlg::OnBnClickedApply2)
END_MESSAGE_MAP()

BOOL CSalnikNestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_pDoc == nullptr) {
        AfxMessageBox(L"Ошибка: указатель на документ не установлен!");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_comboSalnikNest.ResetContent();
    for (int i = 1; i <= 6; i++) {
        CString str;
        str.Format(L"%d", i);
        m_comboSalnikNest.AddString(str);
    }

    int savedIsp = m_pDoc->m_SelectedIsp;
    if (savedIsp >= 1 && savedIsp <= 6) {
        m_comboSalnikNest.SetCurSel(savedIsp - 1);
        UpdateEdit(savedIsp);
    }
    else {
        m_comboSalnikNest.SetCurSel(0);
        UpdateEdit(1);
    }

    return TRUE;
}

void CSalnikNestDlg::OnCbnSelchangeComboPick2()
{
    int idx = m_comboSalnikNest.GetCurSel();
    if (idx != CB_ERR) {
        int var = idx + 1; // 1–6
        UpdateEdit(var);
    }
}

void CSalnikNestDlg::OnBnClickedApply2()
{
    if (m_pDoc == nullptr) return;

    int idx = m_comboSalnikNest.GetCurSel();
    if (idx == CB_ERR) return;

    int selectedIsp = idx + 1;
    m_pDoc->m_SelectedIsp = selectedIsp;
    m_pDoc->m_AssemblyType = selectedIsp;
    m_pDoc->SetParams();              

    AfxMessageBox(L"Исполнение сохранено!");
}

void CSalnikNestDlg::OnBnClickedButtonDo2()
{
    if (m_pDoc == nullptr) {
        AfxMessageBox(L"Ошибка: указатель на документ не установлен!");
        return;
    }

    int idx = m_comboSalnikNest.GetCurSel();
    if (idx != CB_ERR) {
        int var = idx + 1;
        m_pDoc->m_SelectedIsp = var;
        m_pDoc->m_AssemblyType = var;
        m_pDoc->SetParams();
    }

    m_pDoc->LaunchKompas();
    m_pDoc->pKompasApp5 = nullptr;
}

void CSalnikNestDlg::UpdateEdit(int var)
{
    if (m_pDoc == nullptr)
        return;

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
}