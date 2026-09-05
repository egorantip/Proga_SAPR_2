#pragma once
#include <afxcview.h>
// Просмотр CMyTreeView

class CSditestDoc;


class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // защищенный конструктор, используемый при динамическом создании
	
	virtual ~CMyTreeView();
	
	
public:
	CSditestDoc* m_pDoc;
	HTREEITEM m_hRoot, m_hCoord, m_hHatch, m_hSinus;

	void UpdateTreeCheckState(HTREEITEM hItem, BOOL bChecked);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	void FillTree();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPreviewClose();
	afx_msg void OnColor();
};


