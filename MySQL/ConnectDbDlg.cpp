// ConnectDbDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MySQL.h"
#include "ConnectDbDlg.h"
#include "MySQLDoc.h"

// CConnectDbDlg диалоговое окно

IMPLEMENT_DYNAMIC(CConnectDbDlg, CDialogEx)

CConnectDbDlg::CConnectDbDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONNECT_DB, pParent)
	, m_host(_T("127.0.0.1"))
	, m_port(3307)
	, m_user(_T("root"))
	, m_password(_T(""))
	, m_database(_T("mydb"))
{
}

void CConnectDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOST, m_host);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_USER, m_user);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_DATABASE, m_database);
	DDV_MinMaxInt(pDX, m_port, 1, 65535);
}

BEGIN_MESSAGE_MAP(CConnectDbDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConnectDbDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CConnectDbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Загружаем сохраненные параметры
	// Используем безопасный способ получения документа
	CDocument* pDoc = nullptr;
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	if (pos != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		if (pTemplate != NULL)
		{
			pos = pTemplate->GetFirstDocPosition();
			if (pos != NULL)
			{
				pDoc = pTemplate->GetNextDoc(pos);
			}
		}
	}
	
	CMySQLDoc* pMySQLDoc = DYNAMIC_DOWNCAST(CMySQLDoc, pDoc);
	if (pMySQLDoc)
	{
		m_host = pMySQLDoc->m_dbHost;
		m_port = pMySQLDoc->m_dbPort;
		m_user = pMySQLDoc->m_dbUser;
		m_password = pMySQLDoc->m_dbPassword;
		m_database = pMySQLDoc->m_dbName;
		UpdateData(FALSE);
	}

	return TRUE;
}

void CConnectDbDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_host.IsEmpty() || m_user.IsEmpty() || m_database.IsEmpty())
	{
		AfxMessageBox(_T("Заполните все обязательные поля!"));
		return;
	}

	CDocument* pDoc = nullptr;
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	if (pos != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		if (pTemplate != NULL)
		{
			pos = pTemplate->GetFirstDocPosition();
			if (pos != NULL)
			{
				pDoc = pTemplate->GetNextDoc(pos);
			}
		}
	}
	
	CMySQLDoc* pMySQLDoc = DYNAMIC_DOWNCAST(CMySQLDoc, pDoc);
	if (pMySQLDoc)
	{
		pMySQLDoc->m_dbHost = m_host;
		pMySQLDoc->m_dbPort = m_port;
		pMySQLDoc->m_dbUser = m_user;
		pMySQLDoc->m_dbPassword = m_password;
		pMySQLDoc->m_dbName = m_database;
		
		pMySQLDoc->SaveConnectionParams();
		
		if (pMySQLDoc->ConnectToDatabase())
		{
			pMySQLDoc->UpdateAllViews(nullptr);
			CDialogEx::OnOK();
		}
		else
		{
			AfxMessageBox(_T("Не удалось подключиться к базе данных!\r\nПроверьте параметры подключения."));
		}
	}
	else
	{
		AfxMessageBox(_T("Не удалось получить доступ к документу."));
		CDialogEx::OnOK();
	}
}

