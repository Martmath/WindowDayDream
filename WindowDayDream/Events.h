#pragma once
#include <iostream>
#include <sstream>
#include "Triple.h"
#include "MathBase.h"
#include "Mouse.h"
#include "CCatia.h"
#include "FileWork.h"
//#include "MCommand.h"

enum enumRunMode;

template<class T, class... TT>
struct Startable
{
	static constexpr void Start()
	{
		T::Start();
		if constexpr (sizeof...(TT) > 0) Startable<TT...>::Start();
	}
};

struct TFindWindows
{
	static HWND hh;
	static BOOL CALLBACK FindWindowBySubstr(HWND hwnd, LPARAM substring)
	{
		const DWORD TITLE_SIZE = 1024;
		TCHAR windowTitle[TITLE_SIZE];

		if (GetWindowText(hwnd, windowTitle, TITLE_SIZE))
		{
			//_tprintf(TEXT("%s\n"), windowTitle);
			// Uncomment to print all windows being enumerated
			if (_tcsstr(windowTitle, LPCTSTR(substring)) != NULL)
			{
				hh = hwnd;
				// We found the window! Stop enumerating.
				return false;
			}
		}
		return true; // Need to continue enumerating windows
	};

	HWND operator()(const TCHAR text[])//; LPCSTR f)
	{
		hh = 0;
		auto wwe = EnumWindows(FindWindowBySubstr, (LPARAM)text);// (LPARAM)substring);
		return hh;
	};

} static FindWindows;
HWND TFindWindows::hh = 0;


struct TIco
{
	HICON currentIco;
	HICON CATIAIco;
	HWND hwndCATIA=0;

	TIco() {	}

	void Init()
	{
		hwndCATIA = FindWindows("CATIA V5");
		/*if (hwndCATIA > 0) 
		{
			//getDayDreamIco();
			//saveCATIAIco();
			int ee = 8;
		}*/
	}
	void Init(HWND hwndCATIAin)
	{
		hwndCATIA = hwndCATIAin;
		//getDayDreamIco();
		saveCATIAIco();
	};
	
	void getDayDreamIco()
	{
		HINSTANCE hInstance = AfxGetInstanceHandle();
		currentIco = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	};

	void saveCATIAIco()
	{		
		// Get the window icon
		CATIAIco = reinterpret_cast<HICON>(::SendMessage(hwndCATIA, WM_GETICON, ICON_SMALL, 0));
		if (CATIAIco == 0) {
			// Alternative method. Get from the window class
			CATIAIco = reinterpret_cast<HICON>(::GetClassLongPtr(hwndCATIA, GCLP_HICONSM));
		}
	};

	void setDayDreamIco()
	{
		//https://stackoverflow.com/questions/24047470/wm-geticon-not-working-windows
		::SendMessage(hwndCATIA, WM_SETICON, ICON_SMALL, (LPARAM)currentIco);
		::SendMessage(hwndCATIA, WM_SETICON, ICON_BIG, (LPARAM)currentIco);
	};

	void setCATIAIco()
	{
		::SendMessage(hwndCATIA, WM_SETICON, ICON_SMALL, (LPARAM)CATIAIco);
		::SendMessage(hwndCATIA, WM_SETICON, ICON_BIG, (LPARAM)CATIAIco);
		int wwww = 7;
	};
}static ICO;




//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
static std::vector<string> split(const string& s1, const string& s, char delim) {
	std::vector<string> result;
	result.push_back(s1);
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
};


#define DEFINE_ENUM(EnumType, first_value, first_name, ...) \
    enum EnumType { first_name = first_value, __VA_ARGS__ }; \
  const std::vector<std::string> EnumType##Name = split(#first_name, #__VA_ARGS__,',');
DEFINE_ENUM(scenarioN, 0, NO_ACTION, CALIBRATE_HORISONTAL, CALIBRATE_ROTATE, CALIBRATE_VERTICAL,
	SELECT_MODE,MOUSE_MODE,CATIA_MODE);

DEFINE_ENUM(calibrateN, 0, NO_CALIBRATE, CALIBRATE_BYFILE, CALIBRATE_INSESSION);



template<class TData>//int i=0>
struct Events 
{	
	static constexpr int N = getTDeviceN<TData>::N;
	
	struct scCalibrate
	{
		//static TripleXYZ<float> bXYZ;
		static void Start()
		{
			//stlsoft::winstl_project::performance_counter c;		
			//clock_t ww=TData::data.eventData.aTimer.GetTicks();		
			TData::data.bNRun.endClick = pBegClick;
		}

		static void pBegClick()
		{
			TData::data.scenario = scenarioN::CALIBRATE_HORISONTAL;// 1;
			TData::data.agmt.init(TData::data.eventData.bXYZ);
			TData::data.agmt.zFlat.kDelta = TData::data.eventData.bXYZ.z;
			TData::data.agmt.yAlong.kDelta = TData::data.eventData.bXYZ.y;
			TData::data.agmt.xAcross.kDelta = TData::data.eventData.bXYZ.x;

			TData::data.bNRun.endClick = pRightZClick;
			TData::data.eventData.aTimer.Start();
		};

		static void pRightZClick()
		{
			TData::data.eventData.aTimer.Stop();
			if ((TData::data.eventData.aTimer.GetMilliseconds() < 1000) || (TData::data.agmt.isSame(TData::data.eventData.bXYZ)))
			{
				TData::data.bNRun.endClick = pBegClick;
				TData::data.scenario = scenarioN::NO_ACTION;
				saveTo(N, TData::data.agmt.xAcross.kDelta, TData::data.agmt.yAlong.kDelta, TData::data.agmt.zFlat.kDelta,
					TData::data.agmt.xAcross.kMultiply, TData::data.agmt.yAlong.kMultiply, TData::data.agmt.zFlat.kMultiply);

			}
			else
			{
				TData::data.agmt.zFlat.kMultiply = 90.0f / (TData::data.eventData.bXYZ.z - *TData::data.agmt.zFlat.helperValue);
				TData::data.bNRun.endClick = pRollYClick;
				TData::data.scenario = scenarioN::CALIBRATE_ROTATE;
			}
		};

		static void pRollYClick()
		{
			TData::data.agmt.yAlong.kMultiply = 90.0f / (TData::data.eventData.bXYZ.y - *TData::data.agmt.yAlong.helperValue);
			TData::data.bNRun.endClick = pUpXClick;
			TData::data.scenario = scenarioN::CALIBRATE_VERTICAL;

		};

		static void pUpXClick()
		{
			TData::data.agmt.xAcross.kMultiply = 90.0f / (TData::data.eventData.bXYZ.x - *TData::data.agmt.xAcross.helperValue);
			TData::data.bNRun.endClick = pBegClick;

			saveTo(N, TData::data.agmt.xAcross.kDelta, TData::data.agmt.yAlong.kDelta, TData::data.agmt.zFlat.kDelta,
				TData::data.agmt.xAcross.kMultiply, TData::data.agmt.yAlong.kMultiply, TData::data.agmt.zFlat.kMultiply);

			TData::data.scenario = scenarioN::NO_ACTION;
			TData::data.calibrate = calibrateN::CALIBRATE_INSESSION;
		};
	};
	   	  
	struct scCatia
	{
		using T = scCatia;
		static void Start() {	}

		//static void (*runMoveTmp)();
		static void pBegClick()
		{
			ICO.saveCATIAIco();
			ICO.setDayDreamIco();
			Catia.init();			
			
			TData::data.bagmt.runMove = Run;
			TData::data.bNTouch.endClick = pRightButtonClick;

			TData::data.bSoundPlus.endClick = pRollUpClick;
			TData::data.bSoundMinus.endClick = pRollDownClick;


			TData::data.bTouch.beginHolddownClick = pHelperBegClick;
			TData::data.bTouch.endHolddownClick = pHelperEndClick;
		}
		static void pEndClick()
		{
			TData::data.bagmt.runMove = 0;
			TData::data.bNTouch.endClick = 0;
			TData::data.bSoundPlus.endClick = 0;
			TData::data.bSoundMinus.endClick = 0;
			TData::data.bTouch.beginHolddownClick = 0;
			TData::data.bTouch.endHolddownClick = 0;
			ICO.setCATIAIco();
		}

		static const int FF = 2;
		static const int FF_1 = FF - 1;

		
		static constexpr auto smaller90 = [](const double& t)->bool {return abs(t) < 90; };
		static constexpr auto moreMin = [](const double& t)->bool {return abs(t) > 0.001; };
		
		
		
		static void Run()
		{
			TData::data.eventData.nXYZ.x = TData::data.agmt.xAcross.result2();
			TData::data.eventData.nXYZ.y = TData::data.agmt.yAlong.result2();
			TData::data.eventData.nXYZ.z = TData::data.agmt.zFlat.result();
			TData::data.eventData.dXYZ.subtract(TData::data.eventData.nXYZ, TData::data.eventData.oXYZ);

			TData::data.eventData.ini = TData::data.eventData.ini || TData::data.eventData.dXYZ.allTrue(smaller90);				

			if ((TData::data.calibrate !=  calibrateN::NO_CALIBRATE)		
				&& 
				TData::data.eventData.ini && (TData::data.eventData.ff == FF_1)&& TData::data.eventData.dXYZ.someTrue(moreMin));			
			{
				Catia.iniAll(-1 * TData::data.eventData.dXYZ.x, -1 * TData::data.eventData.dXYZ.y, -1 * TData::data.eventData.dXYZ.z);

			}
			if (TData::data.eventData.ff == 0) { 
				TData::data.eventData.oXYZ = TData::data.eventData.nXYZ;
			}
			TData::data.eventData.ff++;
			TData::data.eventData.ff %= FF;
		}

		static void pRightButtonClick() {
			TData::data.eventData.runMoveTmp = TData::data.bagmt.runMove;
			TData::data.bagmt.runMove = 0;
			Catia.initZero();
			Catia.init();
			TData::data.bagmt.runMove = TData::data.eventData.runMoveTmp;
			TData::data.eventData.runMoveTmp = 0;
		}

		static void pRollUpClick() { Catia.runZoomIn(); }
		static void pRollDownClick() { Catia.runZoomOut(); }

		static void pHelperBegClick()
		{
			TData::data.eventData.runMoveTmp = TData::data.bagmt.runMove;
			TData::data.bagmt.runMove = 0;
		}

		static void pHelperEndClick()
		{
			Catia.initZero();
			Catia.init();
			TData::data.bagmt.runMove = TData::data.eventData.runMoveTmp;
			TData::data.eventData.runMoveTmp = 0;
		}
	};
	
	struct scAGMT
	{
		static void Start()
		{
			//bNRun.endHolddownClick = StartAGMTaTouchMove;
			//bNTouch.endClick = StartZ1Z3;//Z1::pBegClick;
		}

		static void pBegClick()
		{
			TData::data.bagmt.runMove = Run;
		}

		static void pEndClick()
		{
			TData::data.bagmt.runMove = 0;
		}

		static void Run()
		{
			mouse.SendMoveF(TData::data.agmt.zFlat.result(), TData::data.agmt.xAcross.result());
		}

		static void RunHolddown()
		{
			mouse.SendMoveHolddownF(TData::data.agmt.zFlat.result(), TData::data.agmt.xAcross.result());
		}
	};

	struct scTouchMove
	{
		//static float bX;
		//static float bY;
		static void Start() {	}


		//static timer aTimer;
		static void pBegClick()
		{
			TData::data.btouch.beginMove = scTouchMove::pBegClickT;
		}

		static void pEndClick()
		{
			TData::data.btouch.beginMove = 0;
			TData::data.btouch.runMove = 0;
		}

		static void pBegClickT()
		{
			TData::data.eventData.bX = (float)TData::data.agmt.xTouch;
			TData::data.eventData.bY = (float)TData::data.agmt.yTouch;
			TData::data.btouch.runMove = Run;
		}

		static void Run()
		{
			mouse.td_X = mouse.td_X + (toF(TData::data.agmt.xTouch) - TData::data.eventData.bX) * 2.0f;
			mouse.td_Y = mouse.td_Y + (toF(TData::data.agmt.yTouch) - TData::data.eventData.bY) * 2.0f;
			TData::data.eventData.bX = toF(TData::data.agmt.xTouch);
			TData::data.eventData.bY = toF(TData::data.agmt.yTouch);
			mouse.SendMoveF(TData::data.agmt.zFlat.result(), TData::data.agmt.xAcross.result());
		}

		static void RunHolddown()
		{
			mouse.td_X = mouse.td_X + ((float)TData::data.agmt.xTouch - TData::data.eventData.bX) * 2.0f;
			mouse.td_Y = mouse.td_Y + ((float)TData::data.agmt.yTouch - TData::data.eventData.bY) * 2.0f;
			TData::data.eventData.bX = (float)TData::data.agmt.xTouch;		TData::data.eventData.bY = (float)TData::data.agmt.yTouch;
			mouse.SendMoveHolddownF(TData::data.agmt.zFlat.result(), TData::data.agmt.xAcross.result());
		}
	};
	   
	struct scHoldDownTouch
	{
		using T = scHoldDownTouch;

		static void Start() {		}
		//static void (*runMoveTmp)();
		//static void (*runMoveTouchTmp)();

		static void pBegClick()
		{
			TData::data.bTouch.beginHolddownClick = scHoldDownTouch::pHelperBegClick;
			TData::data.bTouch.endHolddownClick = scHoldDownTouch::pHelperEndClick;
		}

		static void pEndClick()
		{
			TData::data.bTouch.beginHolddownClick = 0;
			TData::data.bTouch.endHolddownClick = 0;
		}

		static void pHelperBegClick()
		{
			TData::data.eventData.runMoveTmp = TData::data.bagmt.runMove;
			TData::data.eventData.runMoveTouchTmp = TData::data.btouch.runMove;
			TData::data.bagmt.runMove = scAGMT::RunHolddown;
			TData::data.btouch.runMove = scTouchMove::RunHolddown;
		}

		static void pHelperEndClick()
		{
			mouse.LeftClickUp();
			TData::data.bagmt.runMove = TData::data.eventData.runMoveTmp;
			TData::data.btouch.runMove = TData::data.eventData.runMoveTouchTmp;
			TData::data.eventData.runMoveTmp = 0;
			TData::data.eventData.runMoveTouchTmp = 0;
		}
	};
	   
	struct scButtonsMouse
	{
		using T = scButtonsMouse;
		static void Start() {	}


		static void pBegClick()
		{
			TData::data.bTouch.endClick = pLeftButtonClick;
			TData::data.bNTouch.endClick = pRightButtonClick;
			TData::data.bSoundPlus.endClick = pRollUpClick;
			TData::data.bSoundMinus.endClick = pRollDownClick;
		}
		static void pLeftButtonClick() { mouse.LeftClick(); }
		static void pRightButtonClick() { mouse.RightClick(); }

		static void pRollUpClick() { mouse.rollUpClick(); }
		static void pRollDownClick() { mouse.rollDownClick(); }

		static void pEndClick()
		{
			TData::data.bTouch.endClick = 0;
			TData::data.bNTouch.endClick = 0;
			TData::data.bSoundPlus.endClick = 0;
			TData::data.bSoundMinus.endClick = 0;
		}
	};

	struct scModeManager
	{
		using T = scModeManager;		
		//static enumRunMode runMode;
		static void Start()
		{
			TData::data.bNRun.endHolddownClick = pBegClick;
		}

		static void pBegClick()
		{
			ICO.Init();
			if ((ICO.hwndCATIA != 0) && (TData::data.eventData.runMode == enumRunMode::nothing))
			{				
				TData::data.bTouch.endClick = pRunMouse;
				TData::data.bNTouch.endClick = pRunCatia;
				TData::data.scenario = scenarioN::SELECT_MODE;
			}	
			else if ((ICO.hwndCATIA == 0) && (TData::data.eventData.runMode == enumRunMode::nothing))
			{
				pRunMouse();				
			}
			else if (TData::data.eventData.runMode == enumRunMode::mouseRM) { pStopMouse(); }
			else { pStopCatia(); }
		}

		static void pBegNoCatiaClick()
		{
			pRunMouse();
		}

		static void pRunMouse()
		{
			TData::data.eventData.runMode = enumRunMode::mouseRM;
			mouse.toZero();
			scAGMT::pBegClick();
			scTouchMove::pBegClick();
			scButtonsMouse::pBegClick();
			scHoldDownTouch::pBegClick();
			TData::data.scenario = scenarioN::MOUSE_MODE;
		}

		static void pStopMouse()
		{
			TData::data.eventData.runMode = enumRunMode::nothing;
			scAGMT::pEndClick();
			scTouchMove::pEndClick();
			scButtonsMouse::pEndClick();
			scHoldDownTouch::pEndClick();
			TData::data.scenario = scenarioN::NO_ACTION;
		}

		static void pRunCatia()
		{
			TData::data.eventData.runMode = enumRunMode::catiaRM;
			scCatia::pBegClick();
			TData::data.scenario = scenarioN::CATIA_MODE;
		}

		static void pStopCatia()
		{
			TData::data.eventData.runMode = enumRunMode::nothing;
			scCatia::pEndClick();
			TData::data.scenario = scenarioN::NO_ACTION;
		}
	};
	   	  	

	using Ss=Startable<scCalibrate, scModeManager>;


};

/*TscData  Events<TMouse>::scData;
TripleXYZ<float> Events<TMouse>::scCalibrate::bXYZ;
timer Events<TMouse>::scCalibrate::aTimer;

void (*Events<TMouse>::scCatia::runMoveTmp)() = 0;
double Events<TMouse>::scCatia::oX = 0; double Events<TMouse>::scCatia::oY = 0; double Events<TMouse>::scCatia::oZ = 0;
double Events<TMouse>::scCatia::nX = 0; double Events<TMouse>::scCatia::nY = 0; double Events<TMouse>::scCatia::nZ = 0;
double Events<TMouse>::scCatia::dX = 0; double Events<TMouse>::scCatia::dY = 0; double Events<TMouse>::scCatia::dZ = 0;
int Events<TMouse>::scCatia::ff = 0;
bool Events<TMouse>::scCatia::ini = false;
float Events<TMouse>::scTouchMove::bX = 0.0;
float Events<TMouse>::scTouchMove::bY = 0.0;

void (*Events<TMouse>::scHoldDownTouch::runMoveTmp)() = 0;
void (*Events<TMouse>::scHoldDownTouch::runMoveTouchTmp)() = 0;

Events<TMouse>::scModeManager::enumRunMode Events<TMouse>::scModeManager::runMode = Events<TMouse>::scModeManager::nothing;*/

