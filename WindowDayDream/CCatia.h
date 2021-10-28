#pragma once
#include "MCatia.h"
#include <map>
#include <vector>
#include <iostream>
//https://groups.google.com/forum/#!topic/microsoft.public.excel/6QjQefwNv0o
static HRESULT OLEMethod(int autoType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...)
{
	static char g_Fm2nbookTitle[] = { "my application" };
	// Begin variable-argument list...
	va_list marker;
	va_start(marker, cArgs);
	char buf[2000];
	char szName[2000];

	// Convert down to ANSI
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	if (!pDisp)
	{
		sprintf_s(buf, sizeof(buf), "ERROR: NULL IDispatch passed to OLEMethod() for \"%s\" (OLEMethod).", szName);
		strcat_s(buf, sizeof(buf), "\n\nIf there are any warning messages in Excel then please\n"
			"clear them and restart the data transfer.\n\n");
		//strcat_s(buf, sizeof(buf), "Activity: ");
		//strcat_s(buf, sizeof(buf), errorStr.c_str());
		MessageBox(NULL, buf, g_Fm2nbookTitle, 0x10010);
		// _exit (0);
	}

	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr = -1;

	// Get DISPID for name passed...
	if (pDisp)
		hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if (FAILED(hr))
	{
		sprintf_s(buf, sizeof(buf), "ERROR: IDispatch::GetIDsOfNames (\"%s\") failed w/err 0x%08lx (OLEMethod).",
			szName, hr);
		strcat_s(buf, sizeof(buf), "\n\nIf there are any warning messages in Excel then please\n"
			"clear them and restart the data transfer.\n\n");
		MessageBox(NULL, buf, g_Fm2nbookTitle, 0x10010);
		// _exit(0);
		return hr;
	}

	// Allocate memory for arguments...
	VARIANT* pArgs = new VARIANT[cArgs + 1];
	// Extract arguments...
	for (int i = 0; i < cArgs; i++)
	{
		pArgs[i] = va_arg(marker, VARIANT);
	}

	// Build DISPPARAMS
	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	// Handle special-case for property-puts!
	if (autoType & DISPATCH_PROPERTYPUT)
	{
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	// Make the call!  try up to 10 times and then quit after notifying user
	hr = -1;
	int counter = 0;
	while (FAILED(hr) && counter <= 10)
	{
		hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
		// if we failed then sleep for half a second
		if (FAILED(hr))
			Sleep(500);
		counter++;
	}
	if (FAILED(hr))
	{
		sprintf_s(buf, sizeof(buf), "ERROR: IDispatch::Invoke (\"%s\"=%08lx) failed w/err 0x%08lx (OLEMethod).",
			szName, dispID, hr);
		strcat_s(buf, sizeof(buf), "\n\nIf there are any warning messages in CATIA then please\n"
			"clear them and restart the data transfer.\n\n");
		MessageBox(NULL, buf, g_Fm2nbookTitle, 0x10010);
		// _exit(0);
		return hr;
	}
	// End variable-argument section...
	va_end(marker);
	delete[] pArgs;
	return hr;
}


class CCatia
{
	bool isInit = false;
	bool ismultiProc = false;
	HRESULT hr = S_OK;
	IDispatch* actViewer = nullptr;
	IDispatch* actViewpoint = nullptr;
	IStream* streamActViewer = nullptr;
	TAxis ViewAxis{};
	inline static const TVector VVx{ 1, 0, 0 };
	inline static const TVector VVy{ 0, 1, 0 };
	inline static const TVector VVz{ 0, 0, 1 };
	//std::vector<IDispatch*> actViewers;
	std::map<int, IDispatch*> actViewers;
	
	IDispatch* marshalIActViewer(int key)
	{
		//CoGetInterfaceAndReleaseStream(CAT_.g_pStmPtr, IID_IDispatch, (void**)streamActViewer);
		//CoMarshalInterThreadInterfaceInStream(IID_IDispatch, ActViewpoint, &streamActViewer);

		//CreateStreamOnHGlobal(NULL, true, &streamActViewer);
		//CoMarshalInterface(streamActViewer, IID_IDispatch, ActViewer, MSHCTX_INPROC, NULL, MSHLFLAGS_NORMAL);

		auto it = actViewers.find(key);

		if (it == actViewers.end())
		{
			streamActViewer->Seek({ 0 ,0 }, STREAM_SEEK_SET, NULL);
			IDispatch* ActViewer;
			CoUnmarshalInterface(streamActViewer, IID_IDispatch, reinterpret_cast<void**>(&ActViewer));

			actViewers.insert(std::make_pair(key, ActViewer));
			return ActViewer;
		}
		else return it->second;
	};

	class cUpdate
	{
		//IDispatch* actViewer=0;
		DISPID UpdateID = 0;
		DISPPARAMS dataUpdate = { new VARIANT[0], NULL, 0, 0 };
	public:
		cUpdate() {}
		void init(IDispatch* actViewer)
		{
			//actViewer = ActViewer;
			LPOLESTR ptUpdate = (LPOLESTR)L"Update";
			actViewer->GetIDsOfNames(IID_NULL, &ptUpdate, 1, LOCALE_USER_DEFAULT, &UpdateID);
		}

		int counter = 0;
		HRESULT hr = E_FAIL;
		VARIANT result;
		void operator()(IDispatch* actViewer)
		{
			counter = 0;
			hr = E_FAIL;
			while (FAILED(hr) && counter <= 10)
			{
				hr = actViewer->Invoke(UpdateID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dataUpdate, &result, NULL, NULL);
				if (FAILED(hr))		Sleep(500);
				counter++;
			}
		}
	};

	class cZoomOut
	{
		//IDispatch* actViewer=0;
		DISPID ZoomOutID = 0;
		DISPPARAMS dataUpdate = { new VARIANT[0], NULL, 0, 0 };
	public:
		cZoomOut() {}
		void init(IDispatch* actViewer)
		{
			//actViewer = ActViewer;
			LPOLESTR ptUpdate = (LPOLESTR)L"ZoomOut";
			actViewer->GetIDsOfNames(IID_NULL, &ptUpdate, 1, LOCALE_USER_DEFAULT, &ZoomOutID);
		}

		int counter = 0;
		HRESULT hr = -1;
		VARIANT result;
		void operator()(IDispatch* actViewer)
		{
			counter = 0;
			hr = E_FAIL;
			while (FAILED(hr) && counter <= 10)
			{
				hr = actViewer->Invoke(ZoomOutID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dataUpdate, &result, NULL, NULL);
				if (FAILED(hr))		Sleep(500);
				counter++;
			}
		}
	};

	class cZoomIn
	{
		//IDispatch* actViewer=0;
		DISPID ZoomInID = 0;
		DISPPARAMS dataUpdate = { new VARIANT[0], NULL, 0, 0 };
	public:
		cZoomIn() {}
		void init(IDispatch* actViewer)
		{
			//actViewer = ActViewer;
			LPOLESTR ptUpdate = (LPOLESTR)L"ZoomIn";
			actViewer->GetIDsOfNames(IID_NULL, &ptUpdate, 1, LOCALE_USER_DEFAULT, &ZoomInID);
		}

		int counter = 0;
		HRESULT hr = -1;
		VARIANT result;
		void operator()(IDispatch* actViewer)
		{
			counter = 0;
			hr = E_FAIL;
			while (FAILED(hr) && counter <= 10)
			{
				hr = actViewer->Invoke(ZoomInID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dataUpdate, &result, NULL, NULL);
				if (FAILED(hr))		Sleep(500);
				counter++;
			}
		}
	};




	class cRotate
	{
		//IDispatch* actViewer=nullptr;
		DISPID RotateID = 0;
		VARIANT* pV = nullptr;
		DISPPARAMS dataRotate;
	public:
		//DOUBLE& angle = dataRotate.rgvarg[0].dblVal; -not work for xyz. todo - find why??? & how clean
		DOUBLE* angle = nullptr;
		DOUBLE* x = nullptr;
		DOUBLE* y = nullptr;
		DOUBLE* z = nullptr;
		cRotate() { TRACE("cRotate()\n"); };
		void init(IDispatch* actViewer)
		{
			TRACE("cRotate(IDispatch* actViewer)\n");
			dataRotate = { new VARIANT[2], NULL, 2, 0 };
			//actViewer = ActViewer;
			LPOLESTR ptRotate = (LPOLESTR)L"Rotate";
			actViewer->GetIDsOfNames(IID_NULL, &ptRotate, 1, LOCALE_USER_DEFAULT, &RotateID);
			VARIANT& Axis = dataRotate.rgvarg[1];
			VARIANT& Angle = dataRotate.rgvarg[0];
			VariantInit(&Angle);
			Angle.vt = VT_R8;
			Angle.dblVal = 0;

			Axis.vt = VT_ARRAY | VT_VARIANT;

			SAFEARRAYBOUND sab[1];
			sab[0].lLbound = 0; sab[0].cElements = 3;
			Axis.parray = SafeArrayCreate(VT_VARIANT, 1, sab);

			for (int i = 0; i < 3; ++i)
			{
				VARIANT t;
				t.vt = VT_R8;
				t.dblVal = 0;
				LONG l = i;
				hr = SafeArrayPutElement(Axis.parray, &l, (void*)&t);
			}
			hr = SafeArrayAccessData(Axis.parray, (void**)&pV);
			x = &pV[0].dblVal;
			y = &pV[1].dblVal;
			z = &pV[2].dblVal;
			angle =  &dataRotate.rgvarg[0].dblVal;
		};

		void Clear() {
			if (RotateID == 0) {
				TRACE("~cRotate ID=0\n");
				delete[] dataRotate.rgvarg;
			}
			else {
				//std::cout << "Hello, world!";
				RotateID = 0;
				TRACE("~cRotate\n");
				VARIANT& Axis = dataRotate.rgvarg[1];
				SafeArrayUnaccessData(Axis.parray);
				SafeArrayDestroyData(Axis.parray);
				SafeArrayDestroy(Axis.parray);
				////VariantClear(&dataRotate.rgvarg[1]);		//VariantClear(&dataRotate.rgvarg[0]);
				delete[] dataRotate.rgvarg;
				VariantClear(&result);
			}
		
		}
		~cRotate()
		{
			Clear();
		};

		int counter = 0;
		HRESULT hr = -1;
		VARIANT result;

		void operator()(IDispatch* actViewer)
		{
			counter = 0;
			hr = -1;
			while (FAILED(hr) && counter <= 10)
			{
				hr = actViewer->Invoke(RotateID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dataRotate, &result, NULL, NULL);
				if (FAILED(hr)) Sleep(500);
				counter++;
			}
		};

		void operator()(IDispatch* actViewer, DOUBLE X_, DOUBLE TMouse, DOUBLE Z, DOUBLE Angle)
		{
			*x = X_;
			*y = TMouse;
			*z = Z;
			*angle = Angle;
			//Start(actViewer);
			(this)->operator()(actViewer);
		};

	};

	cRotate  rotate;// (ActViewer);
	cUpdate  update;// (ActViewer);

	void streamActViewerActivate()
	{
		//CoGetInterfaceAndReleaseStream(CAT_.g_pStmPtr, IID_IDispatch, (void**)streamActViewer);
		//CoMarshalInterThreadInterfaceInStream(IID_IDispatch, ActViewpoint, &streamActViewer);
		ismultiProc = true;
		CreateStreamOnHGlobal(NULL, true, &streamActViewer);
		CoMarshalInterface(streamActViewer, IID_IDispatch, actViewer, MSHCTX_INPROC, NULL, MSHLFLAGS_NORMAL);
	};

	void Clear() 
	{
		if (actViewer != nullptr) actViewer->Release();
		if (actViewpoint != nullptr) actViewpoint->Release();		
		for (std::map<int, IDispatch*>::iterator it = actViewers.begin(); it != actViewers.end(); ++it)
		{
			it->second->Release();
		}
		actViewers.clear();
		if (streamActViewer != nullptr) streamActViewer->Release();
		rotate.Clear();
	
	}
	
	
	
	void iniAll(IDispatch* actViewer, double PitchX, double RollY, double YawZ)
	{
		//TRACE("CCatia.initALL before\n");
		//::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		TAxis BaseAxis = Math3D::TransformAxis(ViewAxis);
		PAxis BaseAxisP = Math3D::GPAxisRet(BaseAxis);

		/*auto qx = Math3D::createQuatRet(VVx, Math3D::Deg2Rad(RollY));
		auto qy = Math3D::createQuatRet(VVy, Math3D::Deg2Rad(PitchX));
		auto qz = Math3D::createQuatRet(VVz, Math3D::Deg2Rad(YawZ));
		TQuat q_tmp = Math3D::quatMulQuatRet(qz, qy);
		TQuat qq = Math3D::quatMulQuatRet(q_tmp, qx);*/

		TQuat qqq = Math3D::quatFromAnglesRad(Math3D::Deg2Rad(RollY), Math3D::Deg2Rad(PitchX), Math3D::Deg2Rad(YawZ));

		TVector VV; double a;
		Math3D::toAxisAngle(qqq, VV, a);

		TVector VV2 = Math3D::VectorToOtherAxisRet(BaseAxis, VV);
		auto q = Math3D::createQuatRet(VV2, a);

		PAxis BaseAxisP1 = Math3D::quatTransformPAxisRet(q, BaseAxisP);

		TAxis tta = Math3D::GTAxisRet(BaseAxisP1);

		ViewAxis = Math3D::TransformAxis(tta);

		runRotate(actViewer, VV.x, VV.y, VV.z, Math3D::Rad2Deg(a));
		//TRACE("CCatia.initALL END\n");
	};

	cZoomIn  zoomIn;// (ActViewer);
	cZoomOut   zoomOut;// (ActViewer);
public:

	
	CCatia() {
		TRACE("CCatia()\n");
	}
	~CCatia() {
		Clear();
		TRACE("~CCatia()\n");
	}


	void initZero()
	{
		isInit = false;
		Clear();
	};



	void runZoomIn()
	{
		zoomIn(actViewer);
	};

	void runZoomIn(int procKey)
	{
		zoomIn(marshalIActViewer(procKey));
	};



	void runZoomOut()
	{
		zoomOut(actViewer);
	};

	void runZoomOut(int procKey)
	{
		zoomOut(marshalIActViewer(procKey));
	};


	void runRotate(IDispatch* actViewer)
	{
		rotate(actViewer);
		update(actViewer);
	};

	void runRotate(IDispatch* actViewer, DOUBLE X_, DOUBLE TMouse, DOUBLE Z, DOUBLE Angle)
	{
		rotate(actViewer, X_, TMouse, Z, Angle);
		update(actViewer);
	};

	
	template <bool iniMultiThread=false>
	void init()
	{
		TRACE("CCatia.init beg\n");
		if (isInit) return;
		isInit = true;
		CLSID AppClsid;
		IDispatch* pApp;
		IDispatch* ActWin;
	

		//::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		::CoInitialize(NULL);

		::CLSIDFromProgID(L"CATIA.Application", &AppClsid); //get the unique id of CATIA  

		hr = CoCreateInstance(AppClsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pApp);

		VARIANT result, buffer;
		VariantInit(&result);

		hr = OLEMethod(DISPATCH_PROPERTYGET | DISPATCH_METHOD, &result, pApp, (LPOLESTR)L"ActiveWindow", 0);
		ActWin = result.pdispVal;

		hr = OLEMethod(DISPATCH_PROPERTYGET | DISPATCH_METHOD, &result, ActWin, (LPOLESTR)L"ActiveViewer", 0);
		actViewer = result.pdispVal;

		hr = OLEMethod(DISPATCH_PROPERTYGET | DISPATCH_METHOD, &result, actViewer, (LPOLESTR)L"Viewpoint3D", 0);
		actViewpoint = result.pdispVal;

		hr = OLEMethod(DISPATCH_PROPERTYGET | DISPATCH_METHOD, &result, actViewpoint, (LPOLESTR)L"FocusDistance", 0);
		double Focus = result.dblVal;

		VARIANT arr;
		arr.vt = VT_ARRAY | VT_VARIANT | VT_BYREF;

		SAFEARRAYBOUND sab[1];
		sab[0].lLbound = 0; sab[0].cElements = 3;
		SAFEARRAY* ww = SafeArrayCreate(VT_VARIANT, 1, sab);
		arr.pparray = &ww;

		VARIANT* pVals;
		HRESULT hr = SafeArrayAccessData(ww, (void**)&pVals);

		hr = OLEMethod(DISPATCH_METHOD, &result, actViewpoint, (LPOLESTR)L"GetSightDirection", 1, arr);

		ViewAxis.YA.x = pVals[0].dblVal;
		ViewAxis.YA.y = pVals[1].dblVal;
		ViewAxis.YA.z = pVals[2].dblVal;

		hr = OLEMethod(DISPATCH_METHOD, &result, actViewpoint, (LPOLESTR)L"GetUpDirection", 1, arr);

		ViewAxis.ZA.x = pVals[0].dblVal;
		ViewAxis.ZA.y = pVals[1].dblVal;
		ViewAxis.ZA.z = pVals[2].dblVal;

		hr = OLEMethod(DISPATCH_METHOD, &result, actViewpoint, (LPOLESTR)L"GetOrigin", 1, arr);

		ViewAxis.p.x = pVals[0].dblVal + (Focus * ViewAxis.YA.x);
		ViewAxis.p.y = pVals[1].dblVal + (Focus * ViewAxis.YA.y);
		ViewAxis.p.z = pVals[2].dblVal + (Focus * ViewAxis.YA.z);
		ViewAxis.XA = Math3D::VProduct(ViewAxis.YA, ViewAxis.ZA);



		//RotateID = 0;
		//TRACE("~cRotate\n");
		//VARIANT& Axis = dataRotate.rgvarg[1];
		SafeArrayUnaccessData(ww);
		SafeArrayDestroyData(ww);
		SafeArrayDestroy(ww);	
		VariantClear(&result);

		TRACE("CCatia.init before rotate init\n");
		//rotate = cRotate(); update = cUpdate();
		rotate.init(actViewer);		
		update.init(actViewer);
		zoomIn.init(actViewer);
		zoomOut.init(actViewer);
		if constexpr (iniMultiThread) streamActViewerActivate();

		pApp->Release();
		ActWin->Release();




		TRACE("CCatia.init AFTER rotate init\n");
	};	

	void iniAll(double PitchX, double RollY, double YawZ)
	{
		iniAll(actViewer, PitchX, RollY, YawZ);
	};

	void iniAll(int procKey, double PitchX, double RollY, double YawZ)
	{
		iniAll(marshalIActViewer(procKey), PitchX, RollY, YawZ);
	};

} static Catia;