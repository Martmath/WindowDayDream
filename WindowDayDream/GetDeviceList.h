#pragma once
//#include <iostream>
#include "../EnumDevices/Export.h"
#include "../EnumDevices/DeviceDef.h"
//#include <string>
#include "DDConnector.h"
using namespace std;

struct BLEDevice
{
	GUID Guid;
	char* Path;


	static bool get_if1_(DevicePropertiesDN* Properties)
	{		
		return 
		(_tcsstr(
			Properties->Properties[TDevID::COMPATIBLEID::ID].PropertyValue,
			"BTHLEDevice") != NULL) &&
			(_tcsstr(
				Properties->Properties[TDevID::DEVICEDESC::ID].PropertyValue,
				"Bluetooth") != NULL);//rus win get also it
	}

	static const wchar_t* GetWC(char* c, size_t cSize)
	{
		wchar_t* wc = new wchar_t[cSize];
		size_t outSize;
		mbstowcs_s(&outSize, wc, cSize, c, cSize - 1);
		return wc;
	}

	static char* replace_(char* str, char* a, char* b)
	{
		int len = strlen(str);
		int lena = strlen(a), lenb = strlen(b);
		for (char* p = str; p = strstr(p, a); ++p) {
			if (lena != lenb) // shift end as needed
				memmove(p + lenb, p + lena,
					len - (p - str) + lenb);
			memcpy(p, b, lenb);
		}
		return str;
	}

	static std::vector<BLEDevice> getDeviceGUIDS()
	{	
		
		//int iPrev = _CrtSetReportMode(_CRT_ASSERT, 0);
		auto rr = Run(get_if1_, true);
		//_CrtSetReportMode(_CRT_ASSERT, iPrev);
		std::vector<BLEDevice>f;
		for (auto& r : *rr) // access by reference to avoid copying
		{
			const char* U = r->Properties[TDevID::COMPATIBLEID::ID].PropertyValue;
			char* szStrNew = new char[40];
			_tcsncpy_s(szStrNew, 40, U + 12, 39);
			auto EE = new BLEDevice();
			EE->Path = new char[500];

			_tcsncpy_s(EE->Path, 500, r->Properties[TDevID::DEVICEID::ID].PropertyValue, 499);
			EE->Path = replace_(EE->Path, "\\", "#");
			//EE->Path = EE->Path + L"#";

			char* str = new char[500];
			strcpy_s(str, 7, "\\\\?\\");
			strcat_s(str, 300, EE->Path);
			strcat_s(str, 301, "#");
			strcat_s(str, 302, szStrNew);
			//puts(str);
			free(EE->Path);
			EE->Path = str;
			GUID AGuid;
			CLSIDFromString(GetWC(szStrNew, 40), &(EE->Guid));
			free(szStrNew);
			f.push_back(*EE);
		};

		//for (std::vector<DevicePropertiesDN>::iterator itr = rr->begin(); itr != rr->end(); ++itr)	delete (*itr);
		//rr->clear();

		ClearVectorDevices(rr);
		return f;
	}



};


