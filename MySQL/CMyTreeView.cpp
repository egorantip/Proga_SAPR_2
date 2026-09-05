// CMyTreeView.cpp: файл реализации
//


#include "pch.h"
#include "MySQL.h"
#include "CMyTreeView.h"
#include "MySQLDoc.h"
#include "CMyListView.h"
#include "MainFrm.h"

// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyTreeView::OnSelChanged)
END_MESSAGE_MAP()

CMyTreeView::CMyTreeView()
{
	m_hRootItem = nullptr;
	m_hTablesItem = nullptr;
}

CMyTreeView::~CMyTreeView()
{
	// Освобождаем память, выделенную для имен таблиц
	// Делаем это перед уничтожением окна
	if (GetSafeHwnd() != nullptr)
	{
		CTreeCtrl& treeCtrl = GetTreeCtrl();
		CleanupTree(treeCtrl);
	}
}

void CMyTreeView::CleanupItem(HTREEITEM hItem, CTreeCtrl& treeCtrl)
{
	// Обрабатываем дочерние элементы
	HTREEITEM hChild = treeCtrl.GetChildItem(hItem);
	while (hChild != nullptr)
	{
		CleanupItem(hChild, treeCtrl);
		hChild = treeCtrl.GetNextSiblingItem(hChild);
	}
	
	// Освобождаем память, если data указывает на CString
	DWORD_PTR data = treeCtrl.GetItemData(hItem);
	if (data > 2)
	{
		CString* pString = (CString*)data;
		delete pString;
		treeCtrl.SetItemData(hItem, 0);
	}
}

void CMyTreeView::CleanupTree(CTreeCtrl& treeCtrl)
{
	HTREEITEM hItem = treeCtrl.GetRootItem();
	
	while (hItem != nullptr)
	{
		CleanupItem(hItem, treeCtrl);
		hItem = treeCtrl.GetNextSiblingItem(hItem);
	}
}

CMySQLDoc* CMyTreeView::GetDocument() const
{
	return DYNAMIC_DOWNCAST(CMySQLDoc, m_pDocument);
}

void CMyTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	// Настройка стилей дерева
	GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);
	
	RefreshTree();
}

void CMyTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Обновляем дерево при изменении документа
	RefreshTree();
}

void CMyTreeView::RefreshTree()
{
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	
	// Освобождаем память перед удалением элементов
	CleanupTree(treeCtrl);
	
	treeCtrl.DeleteAllItems();
	
	// Получаем документ
	CMySQLDoc* pDoc = GetDocument();
	if (pDoc == nullptr || pDoc->m_conn == nullptr)
		return;
	
	// Создаем корневой элемент
	m_hRootItem = treeCtrl.InsertItem(_T("База данных"), TVI_ROOT);
	treeCtrl.SetItemData(m_hRootItem, 0);
	
	// Получаем список таблиц
	CStringArray tables;
	if (pDoc->GetTablesList(tables))
	{
		for (int i = 0; i < tables.GetSize(); i++)
		{
			CString* pTableName = new CString(tables[i]);
			HTREEITEM hTableItem = treeCtrl.InsertItem(tables[i], m_hRootItem);
			treeCtrl.SetItemData(hTableItem, (DWORD_PTR)pTableName);
			
			// Добавляем колонки таблицы как дочерние элементы
			CStringArray columns;
			if (pDoc->GetTableColumns(tables[i], columns))
			{
				for (int j = 0; j < columns.GetSize(); j++)
				{
					HTREEITEM hColumnItem = treeCtrl.InsertItem(columns[j], hTableItem);
					treeCtrl.SetItemData(hColumnItem, 2);
				}
			}
		}
	}
	
	// Разворачиваем корневой элемент
	treeCtrl.Expand(m_hRootItem, TVE_EXPAND);
}

void CMyTreeView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	HTREEITEM hSelected = treeCtrl.GetSelectedItem();
	
	if (hSelected == nullptr)
	{
		*pResult = 0;
		return;
	}
	
	DWORD_PTR data = treeCtrl.GetItemData(hSelected);
	
	// Проверяем, является ли выбранный элемент таблицей
	// Элементы таблиц имеют data > 2 (указатель на CString)
	// Элементы колонок имеют data == 2
	// Корневой элемент имеет data == 0
	if (data > 2)
	{
		CString* pTableName = (CString*)data;
		if (pTableName != nullptr)
		{
			OnSelectTable(*pTableName);
		}
	}
	else if (data == 2)
	{
		// Выбрана колонка - получаем имя таблицы из родительского элемента
		HTREEITEM hParent = treeCtrl.GetParentItem(hSelected);
		if (hParent != nullptr)
		{
			DWORD_PTR parentData = treeCtrl.GetItemData(hParent);
			if (parentData > 2)
			{
				CString* pTableName = (CString*)parentData;
				if (pTableName != nullptr)
				{
					OnSelectTable(*pTableName);
				}
			}
		}
	}
	
	*pResult = 0;
}

void CMyTreeView::OnSelectTable(const CString& tableName)
{
	// Получаем указатель на ListView через MainFrame
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pMainFrame == nullptr || pMainFrame->m_pListView == nullptr)
		return;
	
	// Отображаем данные таблицы в ListView
	pMainFrame->m_pListView->DisplayTable(tableName);
}

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
