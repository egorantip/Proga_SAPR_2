
// Sdi_testDoc.cpp: реализация класса CSditestDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Sdi_testView.h"
#include "Sdi_test.h"
#endif

#include "Sdi_testDoc.h"

#include "CLinesDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSditestDoc

IMPLEMENT_DYNCREATE(CSditestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSditestDoc, CDocument)
	ON_COMMAND(ID_LINES_COORD, &CSditestDoc::OnLinesCoord)
	ON_COMMAND(ID_LINES_HATCH, &CSditestDoc::OnLinesHatch)
	ON_COMMAND(ID_LINES_SINUS, &CSditestDoc::OnLinesSinus)
	ON_COMMAND(ID_LINES_ROOT, &CSditestDoc::OnLinesRoot)
	ON_UPDATE_COMMAND_UI(ID_LINES_COORD, &CSditestDoc::OnUpdateLinesCoord)
	ON_UPDATE_COMMAND_UI(ID_LINES_HATCH, &CSditestDoc::OnUpdateLinesHatch)
	ON_UPDATE_COMMAND_UI(ID_LINES_SINUS, &CSditestDoc::OnUpdateLinesSinus)
	ON_UPDATE_COMMAND_UI(ID_LINES_ROOT, &CSditestDoc::OnUpdateLinesRoot)
	ON_COMMAND(ID_LINES_MODAL_DLG, &CSditestDoc::OnLinesModalDlg)
END_MESSAGE_MAP()

// Создание или уничтожение CSditestDoc

CSditestDoc::CSditestDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CSditestDoc::~CSditestDoc()
{
}

BOOL CSditestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_pTreeView->FillTree();
	m_bCoord = m_bHatch = m_bSinus = false;

	M_iDataForStorageInOurClass = 100;
	return TRUE;
}




// Сериализация CSditestDoc

void CSditestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
		ar << m_Version;
		ar << M_iDataForStorageInOurClass;
		ar << M_fData;
	}
	else
	{
		ar >> m_Version;

		if (m_Version > 4) 
		{

		}

		ar >> M_iDataForStorageInOurClass;
		ar >> M_fData;
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CSditestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CSditestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CSditestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CSditestDoc

#ifdef _DEBUG
void CSditestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSditestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CSditestDoc

void CSditestDoc::OnLinesCoord()
{
	// TODO: добавьте свой код обработчика команд
	m_bCoord = !m_bCoord;  // Переключаем только текущую кнопку

	// Проверяем, все ли пункты включены - тогда включаем "График"
	if (m_bCoord && m_bHatch && m_bSinus) {
		m_bRoot = TRUE;
	}
	else {
		m_bRoot = FALSE;
	}

	m_pView->Invalidate(TRUE);
	// Обновляем чекбоксы в дереве
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hCoord, m_bCoord);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hRoot, m_bRoot);
}

void CSditestDoc::OnLinesHatch()
{
	// TODO: добавьте свой код обработчика команд
	m_bHatch = !m_bHatch;  // Переключаем только текущую кнопку

	// Проверяем, все ли пункты включены - тогда включаем "График"
	if (m_bCoord && m_bHatch && m_bSinus) {
		m_bRoot = TRUE;
	}
	else {
		m_bRoot = FALSE;
	}

	m_pView->Invalidate(TRUE);
	// Обновляем чекбоксы в дереве
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hHatch, m_bHatch);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hRoot, m_bRoot);
}

void CSditestDoc::OnLinesSinus()
{
	// TODO: добавьте свой код обработчика команд
	m_bSinus = !m_bSinus;  // Переключаем только текущую кнопку

	// Проверяем, все ли пункты включены - тогда включаем "График"
	if (m_bCoord && m_bHatch && m_bSinus) {
		m_bRoot = TRUE;
	}
	else {
		m_bRoot = FALSE;
	}

	m_pView->Invalidate(TRUE);
	// Обновляем чекбоксы в дереве
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hSinus, m_bSinus);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hRoot, m_bRoot);
}

void CSditestDoc::OnUpdateLinesCoord(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetCheck(m_bCoord);
}

void CSditestDoc::OnUpdateLinesHatch(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetCheck(m_bHatch);
}

void CSditestDoc::OnUpdateLinesSinus(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetCheck(m_bSinus);
}


void CSditestDoc::OnLinesRoot()
{
	// TODO: добавьте свой код обработчика команд
	m_bRoot = !m_bRoot;  // Переключаем кнопку "График"

	// При нажатии на "График" устанавливаем/снимаем все галочки
	if (m_bRoot) {
		// Если включаем "График" - включаем все остальные
		m_bCoord = TRUE;
		m_bHatch = TRUE;
		m_bSinus = TRUE;
	}
	else {
		// Если выключаем "График" - выключаем все остальные
		m_bCoord = FALSE;
		m_bHatch = FALSE;
		m_bSinus = FALSE;
	}

	m_pView->Invalidate(TRUE);

	// Обновляем все чекбоксы в дереве
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hRoot, m_bRoot);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hCoord, m_bCoord);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hHatch, m_bHatch);
	m_pTreeView->UpdateTreeCheckState(m_pTreeView->m_hSinus, m_bSinus);
}


void CSditestDoc::OnUpdateLinesRoot(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetCheck(m_bRoot);
}

void CSditestDoc::UpdateTreeView()
{
	if (m_pTreeView != nullptr)
	{
		CTreeCtrl& tree = m_pTreeView->GetTreeCtrl();

		tree.SetCheck(m_pTreeView->m_hSinus, m_bSinus);
		tree.SetCheck(m_pTreeView->m_hHatch, m_bHatch);
		tree.SetCheck(m_pTreeView->m_hCoord, m_bCoord);

		m_pTreeView->Invalidate();
	}
}


void CSditestDoc::OnLinesModalDlg()
{
	// TODO: добавьте свой код обработчика команд
	CLinesDlg dlg;

	dlg.m_bCheckRoot = m_bRoot;
	dlg.m_bCheckCoord = m_bCoord;
	dlg.m_bCheckHatch = m_bHatch;
	dlg.m_bCheckSinus = m_bSinus;

	if (dlg.DoModal() == IDOK)
	{
		// Сохраняем новые значения только если нажали OK
		m_bRoot = dlg.m_bCheckRoot;
		m_bCoord = dlg.m_bCheckCoord;
		m_bHatch = dlg.m_bCheckHatch;
		m_bSinus = dlg.m_bCheckSinus;

		// Обновляем дерево и view
		m_pTreeView->FillTree();
		m_pView->Invalidate(TRUE);
	}

}
