
// SDIProjectCourseView.cpp: реализация класса CSDIProjectCourseView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SDIProjectCourse.h"
#endif

#include "SDIProjectCourseDoc.h"
#include "SDIProjectCourseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIProjectCourseView

IMPLEMENT_DYNCREATE(CSDIProjectCourseView, CView)

BEGIN_MESSAGE_MAP(CSDIProjectCourseView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Создание или уничтожение CSDIProjectCourseView

CSDIProjectCourseView::CSDIProjectCourseView() noexcept
{
	// TODO: добавьте код создания

}

CSDIProjectCourseView::~CSDIProjectCourseView()
{
}

BOOL CSDIProjectCourseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CSDIProjectCourseView

void CSDIProjectCourseView::OnDraw(CDC* pDC)
{
	CSDIProjectCourseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Создание изображений
	CBitmap bmp;
	if (bmp.LoadBitmapW(pDoc->m_pictureToDraw))
	{
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);

		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

		pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
	}
}


// Печать CSDIProjectCourseView

BOOL CSDIProjectCourseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CSDIProjectCourseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CSDIProjectCourseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CSDIProjectCourseView

#ifdef _DEBUG
void CSDIProjectCourseView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIProjectCourseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIProjectCourseDoc* CSDIProjectCourseView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIProjectCourseDoc)));
	return (CSDIProjectCourseDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CSDIProjectCourseView
