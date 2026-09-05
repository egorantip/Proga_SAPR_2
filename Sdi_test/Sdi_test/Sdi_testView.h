
// Sdi_testView.h: интерфейс класса CSditestView
//

#pragma once

#include "Sdi_testDoc.h"
#include "CColorDlg.h"

class CSditestView : public CView
{
protected: // создать только из сериализации
	CSditestView() noexcept;
	DECLARE_DYNCREATE(CSditestView)

// Атрибуты
public:
	CSditestDoc* GetDocument() const;

	CColorDlg m_ColorDlg;

	COLORREF m_Color;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CSditestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnColor();
};

#ifndef _DEBUG  // версия отладки в Sdi_testView.cpp
inline CSditestDoc* CSditestView::GetDocument() const
   { return reinterpret_cast<CSditestDoc*>(m_pDocument); }
#endif

