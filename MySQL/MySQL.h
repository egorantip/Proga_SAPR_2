
// MySQL.h: основной файл заголовка для приложения MySQL
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMySQLApp:
// Сведения о реализации этого класса: MySQL.cpp
//

class CMySQLApp : public CWinApp
{
public:
	CMySQLApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMySQLApp theApp;
