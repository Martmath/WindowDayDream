// EnumDevices.h: основной файл заголовка для библиотеки DLL EnumDevices
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CEnumDevicesApp
// Реализацию этого класса см. в файле EnumDevices.cpp
//

class CEnumDevicesApp : public CWinApp
{
public:
	CEnumDevicesApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
