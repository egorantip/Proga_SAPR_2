// CMyTreeView.cpp: файл реализации
//

#include "pch.h"
#include "Sdi_test.h"
#include "CMyTreeView.h"
#include "Sdi_testDoc.h"
#include "Sdi_testView.h"
#include "CLinesDlg.h"
#include "CColorDlg.h"


// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{

}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, &CMyTreeView::OnPreviewClose)
	ON_COMMAND(ID_COLOR, &CMyTreeView::OnColor)
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
	// Сохраним текущую видимую область/развернутые узлы при желании — опционально

	tree.DeleteAllItems();

	// Вставляем элементы
	m_hRoot = tree.InsertItem(L"График", -1, -1, NULL, TVI_FIRST);
	m_hCoord = tree.InsertItem(L"Координаты", -1, -1, m_hRoot, TVI_LAST);
	m_hHatch = tree.InsertItem(L"Штриховка", -1, -1, m_hRoot, TVI_LAST);
	m_hSinus = tree.InsertItem(L"Синус", -1, -1, m_hRoot, TVI_LAST);

	// Получаем документ — безопасно через GetDocument()
	CSditestDoc* pDoc = dynamic_cast<CSditestDoc*>(GetDocument());
	if (pDoc)
	{
		// Устанавливаем состояния чекбоксов в соответствии с полями документа
		tree.SetCheck(m_hRoot, pDoc->m_bRoot ? TRUE : FALSE);
		tree.SetCheck(m_hCoord, pDoc->m_bCoord ? TRUE : FALSE);
		tree.SetCheck(m_hHatch, pDoc->m_bHatch ? TRUE : FALSE);
		tree.SetCheck(m_hSinus, pDoc->m_bSinus ? TRUE : FALSE);
	}
	else
	{
		// На случай, если нет документа — оставим все unchecked
		tree.SetCheck(m_hRoot, FALSE);
		tree.SetCheck(m_hCoord, FALSE);
		tree.SetCheck(m_hHatch, FALSE);
		tree.SetCheck(m_hSinus, FALSE);
	}

	tree.Expand(m_hRoot, TVE_EXPAND);
}



int CMyTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_CHECKBOXES;

	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Добавьте специализированный код создания

	return 0;
}

void CMyTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CTreeCtrl& tree = GetTreeCtrl();                    // Получаем доступ к дереву элементов
	CTreeView::OnLButtonDown(nFlags, point);            // Стандартная обработка клика

	CRect rc;                                           // Для хранения координат элемента

	tree.GetItemRect(m_hRoot, &rc, false);              // Получаем область корневого элемента
	if (rc.PtInRect(point))
		tree.SelectItem(m_hRoot);

	tree.GetItemRect(m_hCoord, &rc, false);             // Получаем область элемента координат
	if (rc.PtInRect(point))
		tree.SelectItem(m_hCoord);

	tree.GetItemRect(m_hHatch, &rc, false);             // Получаем область элемента штриховки
	if (rc.PtInRect(point))
		tree.SelectItem(m_hHatch);

	tree.GetItemRect(m_hSinus, &rc, false);             // Получаем область элемента синуса
	if (rc.PtInRect(point))
		tree.SelectItem(m_hSinus);

	if (tree.GetSelectedItem() == m_hRoot)              // Если выбран корневой элемент
	{
		bool check = tree.GetCheck(m_hRoot);            // Получаем состояние галочки корня
		tree.SetCheck(m_hCoord, check);
		tree.SetCheck(m_hHatch, check);
		tree.SetCheck(m_hSinus, check);
	}
	else                                                // Если выбран любой другой элемент
		tree.SetCheck(m_hRoot, false);                  // Снимаем галочку с корня


	// Обновляем флаги в документе
	m_pDoc->m_bCoord = tree.GetCheck(m_hCoord);
	m_pDoc->m_bHatch = tree.GetCheck(m_hHatch);
	m_pDoc->m_bSinus = tree.GetCheck(m_hSinus);

	// Проверяем, все ли пункты включены - тогда включаем "График"
	if (m_pDoc->m_bCoord && m_pDoc->m_bHatch && m_pDoc->m_bSinus) {
		tree.SetCheck(m_hRoot, true);
	}

	// Перерисовываем view
	m_pDoc->m_pView->Invalidate();
}

void CMyTreeView::UpdateTreeCheckState(HTREEITEM hItem, BOOL bChecked)
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.SetCheck(hItem, bChecked);
}

void CMyTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CSditestDoc* pDoc = (CSditestDoc*)GetDocument();

	CMenu MainMenu;
	MainMenu.LoadMenuW(IDR_MAINFRAME);

	// Получаем подменю (4-е по счету, индекс 3)
	CMenu* SubMenu = MainMenu.GetSubMenu(3);

	// Преобразуем координаты в экранные
	ClientToScreen(&point);

	// Устанавливаем галочки для пунктов меню в зависимости от состояния документа
	if (pDoc->m_bRoot)
		SubMenu->CheckMenuItem(ID_LINES_ROOT, MF_CHECKED);
	else
		SubMenu->CheckMenuItem(ID_LINES_ROOT, MF_UNCHECKED);
	
	if (pDoc->m_bCoord)
		SubMenu->CheckMenuItem(ID_LINES_COORD, MF_CHECKED);
	else
		SubMenu->CheckMenuItem(ID_LINES_COORD, MF_UNCHECKED);

	if (pDoc->m_bHatch)
		SubMenu->CheckMenuItem(ID_LINES_HATCH, MF_CHECKED);
	else
		SubMenu->CheckMenuItem(ID_LINES_HATCH, MF_UNCHECKED);

	if (pDoc->m_bSinus)
		SubMenu->CheckMenuItem(ID_LINES_SINUS, MF_CHECKED);
	else
		SubMenu->CheckMenuItem(ID_LINES_SINUS, MF_UNCHECKED);

	SubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CView::OnRButtonDown(nFlags, point);
}

// Обработчик команды закрытия предпросмотра
void CMyTreeView::OnPreviewClose()
{

}

void CMyTreeView::OnColor()
{
	// Получаем документ
	CSditestDoc* pDoc = (CSditestDoc*)GetDocument();
	if (!pDoc || !pDoc->m_pView) return;

	// Вызываем ту же функцию, что и в правом виде
	pDoc->m_pView->OnColor();
}