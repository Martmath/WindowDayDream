#pragma warning(push)
#pragma warning(disable:26495)
#pragma once

#include "SetupAPI.h"
#include "DeviceDef.h"
#include "Export.h"
#include <utility>
#include <type_traits>
#include <atlstr.h>
#include "cfgmgr32.h"
#define REGSTR_VAL_MAX_HCID_LEN		1024 
using namespace std;

#define EXPAND( x ) x

#define GET_LAST_ARG(_gla1, _gla2, _gla3, _gla4, _gla5, _gla6, _gla7, _gla8, _gla9, _gla10, _gla11, _glaN,...) _glaN
#define PRINT_MACRO_CHOOSER(_pmc1, _pmc2, _pmc3, _pmc4,_pmc5, _pmc6, _pmc7,_pmc8,_pmc9,_pmc10,_pmc11,...)   EXPAND(GET_LAST_ARG(__VA_ARGS__, \
_pmc11, _pmc10, _pmc9, _pmc8,_pmc7, _pmc6, _pmc5,_pmc4,_pmc3,_pmc2,_pmc1) (__VA_ARGS__)) 

#define Factory(...)  PRINT_MACRO_CHOOSER(Factory_Static_On,Factory_Static_Off,_x,_x,_x,_x,_x,_x,_x,_x,_x,__VA_ARGS__) 

#define Factory_Static_On(ClassName) \
private:\
ClassName() {}\
ClassName(const ClassName&);\
ClassName& operator=(ClassName&);\
public:\
	static ClassName* getInstance() {\
		static ClassName  instance;\
		return &instance;\
	}

#define Factory_Static_Off(ClassName,...) \
private:\
	ClassName() {}\
	ClassName(const ClassName&);\
	ClassName& operator=(ClassName&);\
public:\
	static ClassName* getInstance() { \
	return new ClassName();}


struct getDevices
{
	Factory(getDevices);
private:
	HMACHINE m_hMachine;
	std::function<bool(DevicePropertiesDN*)> get_if = nullptr;
	std::function<void(DEVNODE, GUID, CString)> outData = nullptr;
	bool getALL = true;
	bool foundit = false;
public:
	using ToutData =decltype(outData);
	using Tget_if =decltype(get_if);
	vector<DevicePropertiesDN*>* DeviceProperty;

	inline void InitGetData(Tget_if get_if, bool getALL = true)
	{
		this->getALL = getALL; this->get_if = get_if;
	}

	inline void InitOutData(ToutData outData) { this->outData = outData; }
		inline vector<DevicePropertiesDN*>* EnumDevices()
	{
			if (DeviceProperty != nullptr) {
				for (vector<DevicePropertiesDN*>::iterator itr = DeviceProperty->begin(); itr != DeviceProperty->end(); ++itr)	delete (*itr);			
				DeviceProperty->clear();
			}
			else {

				DeviceProperty = new vector<DevicePropertiesDN*>;
			}
		EnumDevices(DeviceProperty);
		return DeviceProperty;
	}

private:
	inline void EnumDevices(vector<DevicePropertiesDN*>* DeviceProperty)
	{
		TCHAR LocalComputer[MAX_PATH];	DWORD Size = MAX_PATH - 2;
		GetComputerName(LocalComputer + 2, &Size);
		LocalComputer[0] = _T('\\');
		LocalComputer[1] = _T('\\');
		CONFIGRET cr;
		cr = CM_Connect_Machine(LocalComputer, &m_hMachine);
		if (cr != CR_SUCCESS) {
			TCHAR Text[MAX_PATH];
			wsprintf(Text, _T("Machine Connection failed, cr= %lx(hex)\n"), cr);
			return;
		}
		DEVNODE dnRoot;
		CM_Locate_DevNode_Ex(&dnRoot, NULL, 0, m_hMachine);
		DEVNODE dnFirst;
		CM_Get_Child_Ex(&dnFirst, dnRoot, 0, m_hMachine);
		RetrieveSubNodes(dnRoot, NULL, dnFirst, DeviceProperty);
		if (foundit) foundit = false;
		CM_Disconnect_Machine(m_hMachine);
	}

	inline void RetrieveSubNodes(DEVINST parent, DEVINST sibling, DEVNODE dn, vector<DevicePropertiesDN*>* DeviceProperty)
	{
		DEVNODE dnSibling, dnChild;
		do
		{
			CONFIGRET cr = CM_Get_Sibling_Ex(&dnSibling, dn, 0, m_hMachine);
			if (cr != CR_SUCCESS)	dnSibling = NULL;
#ifdef UNICODE
			TCHAR GuidString[MAX_GUID_STRING_LEN];
#else
			UCHAR GuidString[MAX_GUID_STRING_LEN];
#endif
			ULONG Size = sizeof(GuidString);
			cr = CM_Get_DevNode_Registry_Property_Ex(dn, CM_DRP_CLASSGUID, NULL,
				GuidString, &Size, 0, m_hMachine);
			if (cr == CR_SUCCESS)
			{
				GUID Guid;
				GuidString[MAX_GUID_STRING_LEN - 2] = _T('\0');
				UuidFromString(&GuidString[1], &Guid);
				CString DeviceName = GetDeviceName(dn);
				if (outData != nullptr) outData(dn, Guid, DeviceName);
				EnumDeviceProperties(dn, DeviceProperty);
				if (foundit) 	return;
			}
			cr = CM_Get_Child_Ex(&dnChild, dn, 0, m_hMachine);
			if (cr == CR_SUCCESS)	RetrieveSubNodes(dn, NULL, dnChild, DeviceProperty);
			dn = dnSibling;
		} while (dn != NULL);
	}

	inline CString GetDeviceName(DEVNODE DevNode)
	{
		CString	strType, strValue, DisplayName;
		int  BufferSize = MAX_PATH + MAX_DEVICE_ID_LEN;
		ULONG  BufferLen = BufferSize * sizeof(TCHAR);
		LPTSTR	Buffer = strValue.GetBuffer(BufferSize);
		if (CR_SUCCESS == CM_Get_DevNode_Registry_Property_Ex(DevNode,
			CM_DRP_FRIENDLYNAME, NULL, Buffer, &BufferLen, 0, m_hMachine))
		{
			DisplayName = Buffer;
		}
		else
		{
			BufferLen = BufferSize * sizeof(TCHAR);
			if (CR_SUCCESS == CM_Get_DevNode_Registry_Property_Ex(DevNode,
				CM_DRP_DEVICEDESC, NULL, Buffer, &BufferLen, 0, m_hMachine))
				DisplayName = Buffer; 	else	DisplayName = _T("Unknown Device");
		}
		return DisplayName;
	}

	inline  CString GetProperty(DEVNODE dn, ULONG Property)
	{
		CString Temp;
		TCHAR Buffer[REGSTR_VAL_MAX_HCID_LEN] = _T("");
		ULONG Type, Size = sizeof(Buffer);
		if (CM_Get_DevNode_Registry_Property_Ex(dn, Property,
			&Type, Buffer, &Size, 0, m_hMachine) == CR_SUCCESS)
		{
			if (Type == REG_DWORD || Type == REG_MULTI_SZ || Type == REG_SZ)
			{
				if (Type == REG_DWORD)
				{
					DWORD Data = *((DWORD*)Buffer);
					wsprintf(Buffer, _T("0x%08x"), *((DWORD*)Buffer));
				}
				else if (Type == REG_MULTI_SZ)
				{
					LPTSTR p = Buffer;
					while (_T('\0') != *p)
					{
						p += lstrlen(p);
						if (_T('\0') != *p)* p++ = _T(',');
					}
				}
			}
		}
		Temp = Buffer;
		return Temp;
	}

	template<class T, typename  TDevID::have_CM_DRP<T> = 0>
	void initField(DeviceProperties Properties[], CString * Temp = nullptr, DEVNODE * dn = nullptr)
	{
		int yy = T::ID;
		_tcscpy(Properties[T::ID].Property, T::FRIENDLY_NAME);
		*Temp = GetProperty(*dn, T::CM_DRP);
		_tcscpy(Properties[T::ID].PropertyValue, *Temp);
	}

	template<class T, typename  TDevID::no_have_CM_DRP<T> = 0>
	void initField(DeviceProperties Properties[], CString * Temp = nullptr, DEVNODE * dn = nullptr)
	{
		_tcscpy(Properties[T::ID].Property, T::FRIENDLY_NAME);
		if (Temp != nullptr) _tcscpy(Properties[T::ID].PropertyValue, *Temp);
	}

	inline void EnumDeviceProperties(DEVNODE dn, vector<DevicePropertiesDN*>* DeviceProperty)
	{
		int BufferSize = MAX_PATH + MAX_DEVICE_ID_LEN;
		TCHAR Buffer[MAX_PATH + MAX_DEVICE_ID_LEN];
		CString Temp;
		DeviceProperties  Properties[26];
		if (CM_Get_Device_ID_Ex(dn, Buffer, BufferSize, 0, m_hMachine) == CR_SUCCESS)
		{
			_tcscpy(Properties[TDevID::DEVICEID::ID].PropertyValue, Buffer);
		}
		else
		{
			Temp = _T("Fail to retrieve Device ID");
			_tcscpy_s(Properties[TDevID::DEVICEID::ID].PropertyValue, Temp);
		}
		ULONG Status, Problem;
		if (CM_Get_DevNode_Status_Ex(&Status, &Problem, dn, 0, m_hMachine) == CR_SUCCESS)
		{
			Temp.Format(_T("0x%08x"), Status);
			_tcscpy(Properties[TDevID::STATUS::ID].PropertyValue, Temp);
			Temp.Format(_T("0x%08x"), Problem);
			_tcscpy(Properties[TDevID::PROBLEM::ID].PropertyValue, Temp);
		}
		else
		{
			Temp = _T("Fail to retrieve Device Status");
			_tcscpy(Properties[TDevID::STATUS::ID].PropertyValue, Temp);
			Temp = _T("Fail to retrieve Device Problem");
			_tcscpy(Properties[TDevID::PROBLEM::ID].PropertyValue, Temp);
		}
		initField<TDevID::DEVICEID>(Properties);
		initField<TDevID::STATUS>(Properties);
		initField<TDevID::PROBLEM>(Properties);
		initField<TDevID::SERVICE>(Properties, &Temp, &dn);
		initField<TDevID::CAPABILITIES>(Properties, &Temp, &dn);
		initField<TDevID::CONFIGFLAGS>(Properties, &Temp, &dn);
		initField<TDevID::MFG>(Properties, &Temp, &dn);
		initField<TDevID::CLASS>(Properties, &Temp, &dn);
		initField<TDevID::HARDWAREID>(Properties, &Temp, &dn);
		initField<TDevID::CLASSGUID>(Properties, &Temp, &dn);
		initField<TDevID::LOCATION>(Properties, &Temp, &dn);
		initField<TDevID::BUSNUMBER>(Properties, &Temp, &dn);
		initField<TDevID::ENUMERATOR_NAME>(Properties, &Temp, &dn);
		initField<TDevID::DEVICEDESC>(Properties, &Temp, &dn);
		initField<TDevID::FRIENDLYNAME>(Properties, &Temp, &dn);
		initField<TDevID::DRIVER>(Properties, &Temp, &dn);
		initField<TDevID::PHYSICAL_DEVPATH>(Properties, &Temp, &dn);
		initField<TDevID::UI_NUMBER>(Properties, &Temp, &dn);
		initField<TDevID::UPPERFILTERS>(Properties, &Temp, &dn);
		initField<TDevID::LOWERFILTERS>(Properties, &Temp, &dn);
		initField<TDevID::BUSTYPEGUID>(Properties, &Temp, &dn);
		initField<TDevID::LEGACYBUSTYPE>(Properties, &Temp, &dn);
		initField<TDevID::COMPATIBLEID>(Properties, &Temp, &dn);
		Temp = "";
		initField<TDevID::DEVNODE>(Properties, &Temp);
		initField<TDevID::INFOFILE>(Properties, &Temp);
		initField<TDevID::COMPATIBELID>(Properties, &Temp);
		DevicePropertiesDN* dp =new DevicePropertiesDN();
		dp->dn = dn;
		memcpy(dp->Properties, Properties, sizeof(Properties));
		if ((get_if == nullptr) || ((get_if != nullptr) && (get_if(dp))))
		{
			DeviceProperty->push_back(dp);
			if (!getALL) { foundit = true;	return; }
		}
	}
};
