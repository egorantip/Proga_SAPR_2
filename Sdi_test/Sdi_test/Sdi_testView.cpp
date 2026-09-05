
// Sdi_testView.cpp: реализация класса CSditestView
//

#include "pch.h"
#include <vector>
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Sdi_test.h"
#endif

//#include "Sdi_testDoc.h"
#include "Sdi_testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSditestView

IMPLEMENT_DYNCREATE(CSditestView, CView)

BEGIN_MESSAGE_MAP(CSditestView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_RBUTTONDOWN(AFX_ID_PREVIEW_CLOSE, &CSditestView::OnRButtonDown)
    ON_COMMAND(ID_COLOR, &CSditestView::OnColor)
END_MESSAGE_MAP()

// Создание или уничтожение CSditestView

CSditestView::CSditestView() noexcept
{
	// TODO: добавьте код создания

}

CSditestView::~CSditestView()
{
}

BOOL CSditestView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

// Рисование CSditestView
float function(float x, int b) {
	return sin(x) - x - b;
}


void CSditestView::OnDraw(CDC* pDC)
{
	CSditestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
    CRect rc;
    GetClientRect(&rc);
    int32_t width = rc.Width();
    int32_t height = rc.Height();
    double pi = 3.1415;

    CPen newPen(PS_SOLID, 2, m_Color);
    CPen* pOldPen = pDC->SelectObject(&newPen);

    CBrush newBrush(HS_DIAGCROSS, m_Color);
    CBrush* pOldBrush = pDC->SelectObject(&newBrush);

    std::vector<CPoint> sinusoid;
    std::vector<CPoint> poly;

    if (pDoc->m_bCoord)
    {
        pDC->SelectObject(&newPen);
        pDC->MoveTo(0, rc.Height() / 2);
        pDC->LineTo(rc.Width(), rc.Height() / 2);
    }

    if (pDoc->m_bSinus)
    {
        double amplitude = height / 2;
        double frequency = 2 * pi / width;

        for (int x = 0; x < width; x++)
        {
            double y = amplitude * sin(frequency * x);
            sinusoid.emplace_back(x, height / 2 - static_cast<int>(y));
        }

        pDC->SelectObject(&newPen);
        pDC->MoveTo(sinusoid[0]);
        for (int x = 0; x < width; x++)
        {
            pDC->LineTo(sinusoid[x]);
        }
    }

    if (pDoc->m_bHatch && pDoc->m_bSinus)
    {
        for (int x = 0; x < width / 2; x += 15)
        {
            pDC->MoveTo(x, height / 2);
            pDC->LineTo(sinusoid[x]);
            poly.push_back(sinusoid[x + width / 2]);
        }

        poly.emplace_back(width - 1, height / 2);
        poly.emplace_back(width / 2, height / 2);

        pDC->SelectObject(&newBrush);
        pDC->Polygon(poly.data(), poly.size());
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

}


// Печать CSditestView

BOOL CSditestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CSditestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CSditestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CSditestView

#ifdef _DEBUG
void CSditestView::AssertValid() const
{
	CView::AssertValid();
}

void CSditestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSditestDoc* CSditestView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSditestDoc)));
	return (CSditestDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CSditestView

void CSditestView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: добавьте свой код обработчика сообщений или вызов стандартного
    CSditestDoc* pDoc = GetDocument();

    CMenu MainMenu;
    MainMenu.LoadMenuW(IDR_MAINFRAME);

    CMenu* SubMenu = MainMenu.GetSubMenu(3);

    ClientToScreen(&point);

    if (pDoc->m_bCoord && pDoc->m_bHatch && pDoc->m_bSinus)
    {
        SubMenu->CheckMenuItem(ID_LINES_ROOT, MF_CHECKED);  
    }
    else
    {
        SubMenu->CheckMenuItem(ID_LINES_ROOT, MF_UNCHECKED);
    }

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

void CSditestView::OnColor()
{
    // TODO: добавьте свой код обработчика команд
    if (!m_ColorDlg)
    {
        m_ColorDlg.Create(IDD_COLOR_DLG, this);
        m_ColorDlg.m_pView = this;
    }
    
    m_ColorDlg.m_ColorCtrl.SetColor(m_Color);

    m_ColorDlg.ShowWindow(SW_SHOW);
}
