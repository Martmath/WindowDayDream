#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#define _AFXDLL
//#include "stdafx.h"
#include <windows.h>
#include <setupapi.h>
#include <bthdef.h>
#include <Bluetoothleapis.h>
#include <algorithm>
#include "GetDeviceList.h"
#include "OutputG.h"
#include "MathBase.h"
#include <iterator>

using namespace std;
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
#pragma comment(lib, "bthprops.lib")

//#define TO_SEARCH_DEVICE_UUID "{0000FE55-0000-1000-8000-00805F9B34FB}"
//#define TO_SEARCH_DEVICE_UUID "{0000180F-0000-1000-8000-00805F9B34FB}"
template <typename T> void set_cbSize(T &t) {
	if constexpr (std::is_pointer<T>::value) t->cbSize = sizeof(t);
	else t.cbSize = sizeof(t);
}
#define make_init(T,t) T t;set_cbSize(t);
class DDConnector
{
public:
	DDConnector() {};
	~DDConnector() {};
	typedef void(*PFunc)(BTH_LE_GATT_EVENT_TYPE, PVOID, PVOID);
	PFunc m_pFunc;
	inline static HANDLE GetBLEHandle(BLEDevice d) {
		return GetBLEHandle(d.Guid, d.Path);
	}
	inline static HANDLE GetBLEHandle(__in GUID AGuid,char *Pth) {
		GUID BluetoothInterfaceGUID = AGuid;	HANDLE hComm = NULL;
		HDEVINFO hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (hDI == INVALID_HANDLE_VALUE) return NULL;
		make_init(SP_DEVICE_INTERFACE_DATA, did); make_init(SP_DEVINFO_DATA, dinfo); make_init(SP_DEVINFO_DATA, dd);
		for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, 0, &did); i++) {
			make_init(SP_DEVICE_INTERFACE_DETAIL_DATA, DeviceInterfaceDetailData);
			DWORD size = 0;
			if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0)) {
				if (GetLastError() == ERROR_NO_MORE_ITEMS) break;
				PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);
				set_cbSize(pInterfaceDetailData);
				if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd)) break;
				//hComm = CreateFile(pInterfaceDetailData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				hComm = CreateFile(Pth, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if (hComm == INVALID_HANDLE_VALUE)	CloseHandle(hComm);					
				GlobalFree(pInterfaceDetailData);
				if (hComm != 0)	break;
			}
		}
		SetupDiDestroyDeviceInfoList(hDI);
		return hComm;
	}
	HANDLE hLEDevice;
	BLUETOOTH_GATT_EVENT_HANDLE EventHandle;
	
	inline static bool checkGuid(GUID AGuid,char*Pth)
	{
		bool res = false;	
		HANDLE hLEDevice = nullptr;
		hLEDevice = GetBLEHandle(AGuid,Pth);
		USHORT serviceBufferCount;
		PBTH_LE_GATT_SERVICE pServiceBuffer= nullptr;
		if (INVALID_HANDLE_VALUE != hLEDevice)
		{
			HRESULT hr = BluetoothGATTGetServices(hLEDevice, 0, NULL, &serviceBufferCount, BLUETOOTH_GATT_FLAG_NONE);
			if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) goto endcheckGuid;
			pServiceBuffer = (PBTH_LE_GATT_SERVICE)malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
			if (NULL == pServiceBuffer) goto endcheckGuid;
			USHORT numServices;
			hr = BluetoothGATTGetServices(hLEDevice, serviceBufferCount, pServiceBuffer, &numServices, BLUETOOTH_GATT_FLAG_NONE);
			if (S_OK != hr) goto endcheckGuid;
			USHORT charBufferSize;
			hr = BluetoothGATTGetCharacteristics(hLEDevice, pServiceBuffer, 0, NULL, &charBufferSize, BLUETOOTH_GATT_FLAG_NONE);
			if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)  goto endcheckGuid;
			if (charBufferSize == 3)
			{
				PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = NULL;
				free(pCharBuffer);
				res = true;// (charValueDataSize == 28)|| (charValueDataSize == 8);
			}
		}
	endcheckGuid:
		free(pServiceBuffer);
		CloseHandle(hLEDevice);
		//free(hLEDevice);		
		return res;
	}


	inline std::vector<BLEDevice> getDeviceList()
	{
		auto f = BLEDevice::getDeviceGUIDS();
		std::vector<BLEDevice> bar;
		copy_if(f.begin(), f.end(), back_inserter(bar), [=](BLEDevice i)
			{return DDConnector::checkGuid(i.Guid, i.Path); });
		return bar;
	}

	bool r = false;
	inline int init(BLEDevice& bar, PFunc m_pFunc) 
	{
		r = true;
		GUID AGuid = bar.Guid;
		hLEDevice = GetBLEHandle(bar);
		USHORT serviceBufferCount;
		HRESULT hr = BluetoothGATTGetServices(hLEDevice, 0, NULL, &serviceBufferCount, BLUETOOTH_GATT_FLAG_NONE);
		IfPrintLINE(HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr);
		PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
		IfPrintLINE(NULL == pServiceBuffer)
		else RtlZeroMemory(pServiceBuffer, sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
		USHORT numServices;
		hr = BluetoothGATTGetServices(hLEDevice, serviceBufferCount, pServiceBuffer, &numServices, BLUETOOTH_GATT_FLAG_NONE);
		IfPrintLINE(S_OK != hr);
		USHORT charBufferSize;
		hr = BluetoothGATTGetCharacteristics(hLEDevice, pServiceBuffer, 0, NULL, &charBufferSize, BLUETOOTH_GATT_FLAG_NONE);
		IfPrintLINE(HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr);
		PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = NULL;
		//if (charBufferSize > 0) {/*sdfsdfsd*/ } 
		{
			pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
			IfPrintLINE(NULL == pCharBuffer)
			else  RtlZeroMemory(pCharBuffer, charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
			USHORT numChars;
			hr = BluetoothGATTGetCharacteristics(hLEDevice, pServiceBuffer, charBufferSize, pCharBuffer, &numChars, BLUETOOTH_GATT_FLAG_NONE);
			IfPrintLINE(S_OK != hr);
			IfPrintLINE(numChars != charBufferSize);
		}
		PBTH_LE_GATT_CHARACTERISTIC currGattChar;
		for (int i = 0; i < charBufferSize; i++)
		{
			currGattChar = &pCharBuffer[i];
			USHORT charValueDataSize;
			PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;
			USHORT descriptorBufferSize;
			hr = BluetoothGATTGetDescriptors(hLEDevice, currGattChar, 0, NULL, &descriptorBufferSize, BLUETOOTH_GATT_FLAG_NONE);
			IfPrintLINE(HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr);
			PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
			if (descriptorBufferSize > 0)
			{
				pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)malloc(descriptorBufferSize * sizeof(BTH_LE_GATT_DESCRIPTOR));
				IfPrintLINE(NULL == pDescriptorBuffer, (TCHAR*) "pDescriptorBuffer out of memory")
			else RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
			USHORT numDescriptors;
			hr = BluetoothGATTGetDescriptors(hLEDevice, currGattChar, descriptorBufferSize, pDescriptorBuffer, &numDescriptors, BLUETOOTH_GATT_FLAG_NONE);
			IfPrintLINE(S_OK != hr, (TCHAR*) "BluetoothGATTGetDescriptors - Actual Data %d", hr);
			IfPrintLINE(numDescriptors != descriptorBufferSize, (TCHAR*) "buffer size and buffer size actual size mismatch");
			for (int j = 0; j < numDescriptors; j++)
			{
				PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[j];
				USHORT descValueDataSize;
				hr = BluetoothGATTGetDescriptorValue(hLEDevice, currGattDescriptor, 0, NULL, &descValueDataSize, BLUETOOTH_GATT_FLAG_NONE);
				IfPrintLINE(HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr, (TCHAR*)"BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
				PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);
				IfPrintLINE(NULL == pDescValueBuffer, (TCHAR*)"pDescValueBuffer out of memory\r\n")
					else RtlZeroMemory(pDescValueBuffer, descValueDataSize);
					hr = BluetoothGATTGetDescriptorValue(hLEDevice, currGattDescriptor, (ULONG)descValueDataSize, pDescValueBuffer, NULL, BLUETOOTH_GATT_FLAG_NONE);
					IfPrintLINE(S_OK != hr, (TCHAR*)"BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
					if (currGattDescriptor->AttributeHandle < 255)
					{
						BTH_LE_GATT_DESCRIPTOR_VALUE newValue;
						RtlZeroMemory(&newValue, sizeof(newValue));
						newValue.DescriptorType = ClientCharacteristicConfiguration;
						newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
						hr = BluetoothGATTSetDescriptorValue(hLEDevice, currGattDescriptor, &newValue, BLUETOOTH_GATT_FLAG_NONE);
						IfPrintLINE(S_OK != hr, (TCHAR*)"BluetoothGATTGetDescriptorValue - Actual Data %d", hr)
					else _trace((TCHAR*) "setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
					}
			}
			}
			if ((currGattChar->IsNotifiable) && (descriptorBufferSize > 0))// && (currGattChar->IsReadable)) 
			{
				BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;
				BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
				EventParameterIn.Characteristics[0] = *currGattChar;
				EventParameterIn.NumCharacteristics = 1;
				hr = BluetoothGATTRegisterEvent(hLEDevice, EventType, &EventParameterIn, (PFNBLUETOOTH_GATT_EVENT_CALLBACK)m_pFunc, NULL, &EventHandle, BLUETOOTH_GATT_FLAG_NONE);
				IfPrintLINE(S_OK != hr);
				return 0;
			}
		}
		IfretLINE(GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS);		
		return 0;
	}

	inline void stop() { 
		if (r) 
		{
			r = false;
			HRESULT hr = BluetoothGATTUnregisterEvent(EventHandle, BLUETOOTH_GATT_FLAG_NONE);
			CloseHandle(hLEDevice);
		}
	}

};

