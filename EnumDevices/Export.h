#pragma once
#include <windows.h>
#include "cfgmgr32.h"
#include <functional>
#include <tchar.h>
#include <vector>
#include <cstring>
//#define DLL_EXPORT   https://stackoverflow.com/questions/14980649/macro-for-dllexport-dllimport-switch
#ifdef _DLL_EXPORT
#define DLL_PREFIX __declspec(dllexport)
#else
#define DLL_PREFIX __declspec(dllimport)
#endif // DLL_EXPORT

struct DeviceProperties
{
	TCHAR Property[100];
	TCHAR PropertyValue[500];
};

struct DevicePropertiesDN
{
	DEVNODE dn;
	DeviceProperties Properties[26];
};


DLL_PREFIX std::vector<DevicePropertiesDN*>*
Run(std::function<bool(DevicePropertiesDN*)> filter, bool exitAfterFirst = false);


DLL_PREFIX void
ClearVectorDevices(std::vector<DevicePropertiesDN*>* v);