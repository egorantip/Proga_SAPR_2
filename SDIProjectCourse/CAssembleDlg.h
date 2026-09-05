#pragma once
#include "afxdialogex.h"
#include "SDIProjectCourseDoc.h"

class CAssembleDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAssembleDlg)

public:
    CAssembleDlg(CWnd* pParent = nullptr);
    virtual ~CAssembleDlg();
    CSDIProjectCourseDoc* m_pDoc = nullptr;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CAssembleDlg };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

public:
    CComboBox m_comboAssemble;

    CEdit m_editSalnikD;
    CEdit m_editSalnikL;
    CEdit m_editThreadD;
    CEdit m_editThreadP;
    CEdit m_editGaykaSmallD;
    CEdit m_editGaykaL;
    CEdit m_editGaykaD;
    CEdit m_editGaykaS;
    CEdit m_editShaybaD;
    CEdit m_editShaybaSmallD;
    CEdit m_editShaybaS;

    virtual BOOL OnInitDialog();
    void UpdateEdit(int var);

    afx_msg void OnCbnSelchangeComboPick();
    afx_msg void OnBnClickedButtonDo();
    afx_msg void OnBnClickedApply();
};