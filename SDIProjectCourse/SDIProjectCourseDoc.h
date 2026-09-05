
// SDIProjectCourseDoc.h: интерфейс класса CSDIProjectCourseDoc 
//
#pragma once

#include <atlcomcli.h>

#include <filesystem>

#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants.h"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants3D.h"
#import "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\lib\kAPI5.tlb"

using namespace Kompas6API5;

#include "CMyTreeView.h"
class CMyTreeView;
class CSDIProjectCourseView;

class CSDIProjectCourseDoc : public CDocument
{
protected: // создать только из сериализации
	CSDIProjectCourseDoc() noexcept;
	DECLARE_DYNCREATE(CSDIProjectCourseDoc)

// Атрибуты
public:
	CMyTreeView* m_pTreeView;
	CSDIProjectCourseView* m_pView;
	int m_nSelectedItem;
	void SetPicture(int pictureId);
	int m_pictureToDraw;

	CComPtr<IUnknown> pKompasAppUnk = nullptr;
	KompasObjectPtr pKompasApp5;
	ksPartPtr pMainDoc;
	void LaunchKompas(); //запуск компаса
	bool launched = false;

	// Параметры сальника
	double m_Salnik_D;      // Диаметр сальника
	double m_Salnik_L;      // Длина сальника
	double m_ThreadD;		// Диаметр резьбы
	double m_ThreadP;		// Шаг резьбы

	// Параметры гайки
	double m_Gayka_d;       // Внутренний диаметр гайки
	double m_Gayka_L;       // Общая длина гайки
	double m_Gayka_D;       // Диаметр для построения шестиугольника
	double m_Gayka_S;       // Ширина шестиугольника (размер под ключ)

	// Параметры шайбы
	double m_Shayba_D;      // Внешний диаметр шайбы
	double m_Shayba_d;      // Внутренний диаметр отверстия
	double m_Shayba_S;      // Толщина шайбы

	int m_AssemblyType = 1;
	int m_SelectedIsp = 1;

	bool CreateSalnik(ksPartPtr pPart, double D, double L, double threadP, double threadD);
	bool CreateGayka(ksPartPtr pPart, double d, double L, double D, double threadP, double threadD);
	bool CreateShayba(ksPartPtr pPart, double D, double d, double S);
	void SetParams();

	afx_msg void CreateAssembly();
	bool CheckData();

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CSDIProjectCourseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnIddCAssembleDlg();
	afx_msg void OnIddCSalnikNestDlg();
};
