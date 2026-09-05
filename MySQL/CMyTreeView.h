#pragma once
#include "afxcview.h"
#include <mysql.h>
#include "CMyListView.h"
#include "MySQLDoc.h"

// Просмотр CMyTreeView

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CMyTreeView();

	CMySQLDoc* GetDocument() const;
	
public:
	void RefreshTree();
	void OnSelectTable(const CString& tableName);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	HTREEITEM m_hRootItem;
	HTREEITEM m_hTablesItem;
	
	void CleanupItem(HTREEITEM hItem, CTreeCtrl& treeCtrl);
	void CleanupTree(CTreeCtrl& treeCtrl);
	
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};



