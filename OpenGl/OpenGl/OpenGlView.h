
// OpenGlView.h: интерфейс класса COpenGlView
//
#include "GL.h"
#pragma once



class COpenGlView : public CView
{
protected: // создать только из сериализации
	COpenGlView() noexcept;
	DECLARE_DYNCREATE(COpenGlView)

// Атрибуты
public:
	COpenGlDoc* GetDocument() const;
	COpenGL opengl;

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
	virtual ~COpenGlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint m_prevMousePos = {0,0};  // Предыдущая позиция мыши
	bool m_bRotate = false;         // Флаг вращения
	bool m_bPan = false;            // Флаг смещения
// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // версия отладки в OpenGlView.cpp
inline COpenGlDoc* COpenGlView::GetDocument() const
   { return reinterpret_cast<COpenGlDoc*>(m_pDocument); }
#endif

