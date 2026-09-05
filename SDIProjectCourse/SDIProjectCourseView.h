
// SDIProjectCourseView.h: интерфейс класса CSDIProjectCourseView
//
#include "SDIProjectCourseDoc.h"

#pragma once


class CSDIProjectCourseView : public CView
{
protected: // создать только из сериализации
	CSDIProjectCourseView() noexcept;
	DECLARE_DYNCREATE(CSDIProjectCourseView)

// Атрибуты
public:
	CSDIProjectCourseDoc* GetDocument() const;

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
	virtual ~CSDIProjectCourseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в SDIProjectCourseView.cpp
inline CSDIProjectCourseDoc* CSDIProjectCourseView::GetDocument() const
   { return reinterpret_cast<CSDIProjectCourseDoc*>(m_pDocument); }
#endif

