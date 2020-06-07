// EnumDevices.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "EnumDevices.h"
//#include <string>
#pragma comment(lib, "SetupAPI")
//#pragma comment(lib, "BluetoothApis.lib")
#pragma comment (lib, "Rpcrt4.lib")
//#pragma comment(lib, "Coredll")
//#pragma comment (lib, "Setupapi.dll") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnumDevicesApp

BEGIN_MESSAGE_MAP(CEnumDevicesApp, CWinApp)
END_MESSAGE_MAP()

CEnumDevicesApp::CEnumDevicesApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}



CEnumDevicesApp theApp;



BOOL CEnumDevicesApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
