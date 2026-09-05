// CMyTreeView.cpp: файл реализации
//

#include "pch.h"
#include "SDIProjectCourse.h"
#include "CMyTreeView.h"
#include "SDIProjectCourseDoc.h"
#include "CAssembleDlg.h"
#include "CSalnikNestDlg.h"


// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{

}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyTreeView::OnTvnSelchanged)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// Диагностика CMyTreeView

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// Обработчики сообщений CMyTreeView

void CMyTreeView::FillTree() 
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.DeleteAllItems();

	m_hDetailsRoot = tree.InsertItem(L"Детали сборки", -1, -1, NULL, TVI_FIRST);

	m_hSalnikNest = tree.InsertItem(L"Гнездо сальника", -1, -1, m_hDetailsRoot, TVI_LAST);
	m_hGayka = tree.InsertItem(L"Гайка", -1, -1, m_hDetailsRoot, TVI_LAST);
	m_hShayba = tree.InsertItem(L"Шайба", -1, -1, m_hDetailsRoot, TVI_LAST);

	tree.Expand(m_hDetailsRoot, TVE_EXPAND);
}

void CMyTreeView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	if (hSelected == m_hDetailsRoot) {
		m_pDoc->m_nSelectedItem = 0;
		m_pDoc->SetPicture(4); // общая сборка
	}
	else if (hSelected == m_hSalnikNest) {
		m_pDoc->m_nSelectedItem = 1;
		m_pDoc->SetPicture(1); // гнездо сальника
	}
	else if (hSelected == m_hGayka) {
		m_pDoc->m_nSelectedItem = 2;
		m_pDoc->SetPicture(2); // гайка
	}
	else if (hSelected == m_hShayba) {
		m_pDoc->m_nSelectedItem = 3;
		m_pDoc->SetPicture(3); // шайба
	}

	*pResult = 0;
}

void CMyTreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeCtrl& tree = GetTreeCtrl();
	UINT flags = 0;
	HTREEITEM hItem = tree.HitTest(point, &flags);

	CFrameWnd* pFrame = GetParentFrame();
	if (!pFrame) return;

	CSDIProjectCourseDoc* pDoc = static_cast<CSDIProjectCourseDoc*>(pFrame->GetActiveDocument());
	if (!pDoc) return;

	if (hItem == m_hSalnikNest)
	{
		CSalnikNestDlg dlg;
		dlg.m_pDoc = pDoc;
		dlg.DoModal();
	}
	else if (hItem == m_hDetailsRoot)
	{
		CAssembleDlg dlg;
		dlg.m_pDoc = pDoc;
		dlg.DoModal();
	}

	// Обновляем активный вид (например, картинку)
	CView* pActiveView = pFrame->GetActiveView();
	if (pActiveView)
		pActiveView->Invalidate(TRUE);

	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CMyTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hTemp = tree.GetSelectedItem();
	CFrameWnd* wnd = this->GetParentFrame();
	CSDIProjectCourseDoc* pDoc = (CSDIProjectCourseDoc*)wnd->GetActiveDocument();
	CRect rc;

	if (tree.GetItemRect(m_hSalnikNest, &rc, true) && rc.PtInRect(point))
	{
		tree.SelectItem(m_hSalnikNest);
		pDoc->SetPicture(1); // Гнездо
	}
	else if (tree.GetItemRect(m_hGayka, &rc, true) && rc.PtInRect(point))
	{
		tree.SelectItem(m_hGayka);
		pDoc->SetPicture(2); // Гайка
	}
	else if (tree.GetItemRect(m_hShayba, &rc, true) && rc.PtInRect(point))
	{
		tree.SelectItem(m_hShayba);
		pDoc->SetPicture(3); // Шайба
	}
	else
	{
		tree.SelectItem(m_hDetailsRoot);
		pDoc->SetPicture(4);  // Сборочный чертеж
	}

	CTreeView::OnLButtonDown(nFlags, point);
}