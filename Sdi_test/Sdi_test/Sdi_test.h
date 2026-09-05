
// Sdi_test.h: основной файл заголовка для приложения Sdi_test
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CSditestApp:
// Сведения о реализации этого класса: Sdi_test.cpp
//

class CSditestApp : public CWinApp
{
public:
	CSditestApp() noexcept;



// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSditestApp theApp;
