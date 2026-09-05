#pragma once
#include "afxcview.h"
#include <mysql.h>
#include "CMyTreeView.h"
#include "MySQLDoc.h"

// Просмотр CMyListView
class CMainFrame;
class CMyTreeView;
class CMyListView : public CListView
{
	DECLARE_DYNCREATE(CMyListView)

protected:
	CMyListView();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CMyListView();

public:
	CString m_currentTableName;
	CMyTreeView* m_pTree = nullptr;
	CStringArray m_columnNames; // Сохраняем имена колонок для редактирования

	CMySQLDoc* GetDocument() const;
	
	void DisplayTable(const CString& tableName);
	void DisplayQueryResult(const CString& sql);
	void ClearList();
	
	// Inline редактирование
	void StartEditCell(int item, int subItem);
	void EndEditCell(BOOL save);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	// Переменные для inline редактирования
	CEdit* m_pEdit;
	int m_editingItem;
	int m_editingSubItem;
	CString m_primaryKeyColumn;
	CString m_primaryKeyValue;
	CString m_originalValue;
	
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndEditCommand();
protected:
	DECLARE_MESSAGE_MAP()
};


