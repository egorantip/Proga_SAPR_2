
// SDIProjectCourse.h: основной файл заголовка для приложения SDIProjectCourse
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CSDIProjectCourseApp:
// Сведения о реализации этого класса: SDIProjectCourse.cpp
//

class CSDIProjectCourseApp : public CWinApp
{
public:
	CSDIProjectCourseApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSDIProjectCourseApp theApp;
