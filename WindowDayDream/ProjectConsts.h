#pragma once


#pragma warning (disable: 26812)

#define BASE_INTEGRAL 32762 //SHORT_MAX - 5 


//#define NOOUT

//#define MADGWICK_CHECK //not working mode!!! is bad for mouse/catia movings

#define DELEGATE_CHECK

#define ANGLE_TO_SCREEN 75.0 

#define STEP_ROUND 8 

using bigNum = int;
using smallNum = short;
using decimal = float;
static  constexpr decimal decimalMax = (std::numeric_limits<decimal>::max)();
static  constexpr bigNum bigNumMax = (std::numeric_limits<bigNum>::max)();
static  constexpr smallNum smallNumMax = (std::numeric_limits<smallNum>::max)();


unsigned long long cycleCount = 0;
void outEventMessage(const char* name, const char* n) {
	const std::string CC = std::string(std::to_string(++cycleCount) + "_" + n + "_" + name);
	theApp.dlg->m_List.InsertString(0, CC.c_str());//AddString(CC.c_str());
	theApp.dlg->m_List.DeleteString(100);
	////return  CC;
};

inline void DoEvents() {
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && true) {
		if (!AfxGetThread()->PumpMessage())	break;
	}
};