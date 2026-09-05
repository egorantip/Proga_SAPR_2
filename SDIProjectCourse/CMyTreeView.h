#pragma once
#include <afxcview.h>

// Просмотр CMyTreeView

class CSDIProjectCourseDoc;
class CSDIProjectCourseView;

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CMyTreeView();

public:
	HTREEITEM m_hDetailsRoot, m_hSalnikNest, m_hGayka, m_hShayba;
	CSDIProjectCourseDoc* m_pDoc;
	CSDIProjectCourseView* m_pView;
	void FillTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


