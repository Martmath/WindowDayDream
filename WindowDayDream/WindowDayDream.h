
// WindowDayDream.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "WindowDayDreamDlg.h"

// CWindowDayDreamApp:
// See WindowDayDream.cpp for the implementation of this class
//

class CWindowDayDreamApp : public CWinApp
{
public:
	CWindowDayDreamApp();
	CWindowDayDreamDlg* dlg;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWindowDayDreamApp theApp;
