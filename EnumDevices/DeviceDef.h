#ifndef _DEVICE_DEF_H_
#define _DEVICE_DEF_H_
//#pragma once
#include "cfgmgr32.h"
#include <map>
#include <utility>
#include <type_traits>

/*#define have_fieid(x) template <class CheckT, class = void> \
struct is_havefield_##x {\
using no = void; \
static constexpr bool is_have = false;};\
template<class CheckT> \
struct is_havefield_##x<CheckT,std::void_t<decltype(CheckT::##x)>>{\
	using yes = void;\
	static constexpr bool is_have = true; };\
template <class U> using enable_if_t_have_##x = \
std::enable_if_t<is_havefield_CM_DRP<U>::is_have>;\
template <class U> using enable_if_t_no_have_##x = \
std::enable_if_t<!is_havefield_CM_DRP<U>::is_have>;*/


#define have_fieid(x) template <class CheckT, class = void> \
struct is_havefield_##x {\
using no = void; \
static constexpr bool is_have = false;};\
template<class CheckT> \
struct is_havefield_##x<CheckT, std::void_t<decltype(CheckT::##x)>> {\
	using yes = void;\
	static constexpr bool is_have = true; };\
template <class U> using have_##x = typename is_havefield_##x<U>::yes*;\
template <class U> using no_have_##x = typename is_havefield_##x<U>::no*;

struct is_h {
	using  ulong= long;
};

template <typename T=int>
struct TDevice_ID
{	
	have_fieid(CM_DRP);

	struct is_h1 {
		using  ulong= long;
		//friend TDevice_ID<T>;
	};
	//friend TDevice_ID<T>::is_h1;
	using  ulong1 =typename  is_h1::ulong;
	typename is_h1::ulong uu=5;
	//using ulong3=is_h::ulong;
	typedef struct TDevice_ID<T>::is_h1 nested;

	struct bDev {
		bDev(const TCHAR iFRIENDLY_NAME[100], const T iID, const T iCM_DRP):
			//FRIENDLY_NAME(iFRIENDLY_NAME), 
			ID(iID), CM_DRP(iCM_DRP)		
		{
			_tcscpy(FRIENDLY_NAME, iFRIENDLY_NAME);
		}
		bDev(const TCHAR iFRIENDLY_NAME[100], const T iID):
			//FRIENDLY_NAME(iFRIENDLY_NAME),
			ID(iID), CM_DRP(T())		
		{
			_tcscpy(FRIENDLY_NAME, iFRIENDLY_NAME);
		}
		TCHAR FRIENDLY_NAME[100];
		const T ID;
		const T CM_DRP;
	};	

	template <class U, typename have_CM_DRP<U> = 0>
	inline static constexpr TDevice_ID<T>::bDev* baseFactory() {
		return new TDevice_ID<T>::bDev(U::FRIENDLY_NAME, U::ID,U::CM_DRP);
	}
	template <class U, typename no_have_CM_DRP<U> = 0>
	inline static constexpr TDevice_ID<T>::bDev* baseFactory() {
		return new TDevice_ID<T>::bDev(U::FRIENDLY_NAME, U::ID);
	}

	static std::map<T, bDev> baseDevs;
	template <class U>
	struct bDevCreator 
	{	
		static TDevice_ID<T>::bDev* baseFactory() 
		{
			auto it = TDevice_ID<T>::baseDevs.find(U::ID);
			return  (it == TDevice_ID<T>::baseDevs.end())?
				TDevice_ID<T>::baseFactory<U>():it->second;
	}	};


#define  SET_FRIENDLYNAME(x) static constexpr \
TCHAR FRIENDLY_NAME[100] = x
#define  SET_ID(x) static const T ID = x
#define  SET_DRP(x) static const T CM_DRP = x
//	- thats Ok but macros use are enough
#define  SUBSTATICCLASSCREATE(name,x,friendlyName) struct name:bDevCreator<name> \
{ SET_ID(x); SET_DRP(CM_DRP_##name); SET_FRIENDLYNAME(friendlyName)};
	
	struct DEVICEID:bDevCreator<DEVICEID> {
		static const T ID = 0;	
		static constexpr TCHAR FRIENDLY_NAME[100] = "Device ID: ";
		const bDev* Base= TDevice_ID<T>::baseFactory<DEVICEID>();
		using tt =typename DEVICEID;
		static bDev* G() { return std::declval<tt>().Base; }
	};
	struct STATUS :bDevCreator<STATUS> {
		static const T ID = 1;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Status: ";
	};
	struct PROBLEM :bDevCreator<PROBLEM> {
		static const T ID = 2;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Problem: ";
	};
	struct SERVICE :bDevCreator<SERVICE> {
		static const T ID = 3;
		static const T CM_DRP = CM_DRP_SERVICE;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Service: ";
	};
	struct CAPABILITIES :bDevCreator<CAPABILITIES> {
		static const T ID = 4;
		static const T CM_DRP = CM_DRP_CAPABILITIES;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Capabilities: ";
	};
	struct CONFIGFLAGS :bDevCreator<CONFIGFLAGS> {
		static const T ID = 5;
		static const T CM_DRP = CM_DRP_CONFIGFLAGS;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Config Flags: ";
	};
	struct DEVNODE :bDevCreator<DEVNODE> {
		static const T ID = 6;	
		static constexpr TCHAR FRIENDLY_NAME[100] = "DevNode: ";
	};
	struct COMPATIBELID :bDevCreator<COMPATIBELID> {
		static const T ID = 7;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Compatible IDs: ";
	};
	struct CLASS :bDevCreator<CLASS> {
		static const T ID = 8;
		static const T CM_DRP = CM_DRP_CLASS;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Class: ";
	};
	struct INFOFILE :bDevCreator<INFOFILE> {
		static const T ID = 9;	
		static constexpr TCHAR FRIENDLY_NAME[100] = "Inf file: ";
	};
	struct MFG :bDevCreator<MFG> {
		static const T ID = 10;
		static const T CM_DRP = CM_DRP_MFG;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Manufacturer: ";
	};
	struct HARDWAREID :bDevCreator<HARDWAREID> {
		static const T ID = 11;
		static const T CM_DRP = CM_DRP_HARDWAREID;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Hardware IDs: ";
	};
	struct COMPATIBLEID :bDevCreator<COMPATIBLEID> {
		static const T ID = 12;
		static const T CM_DRP = CM_DRP_COMPATIBLEIDS;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Compatible IDs: ";
	};
	struct CLASSGUID :bDevCreator<CLASSGUID> {
		static const T ID = 13;
		static const T CM_DRP = CM_DRP_CLASSGUID;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Class Guid: ";
	};
	struct LOCATION :bDevCreator<LOCATION> {
		static const T ID = 14;
		static const T CM_DRP = CM_DRP_LOCATION_INFORMATION;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Location: ";
	};
	struct BUSNUMBER :bDevCreator<BUSNUMBER> {
		static const T ID = 15;
		static const T CM_DRP = CM_DRP_BUSNUMBER;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Bus number: ";
	};
	struct ENUMERATOR_NAME :bDevCreator<ENUMERATOR_NAME> {
		static const T ID = 16;
		static const T CM_DRP = CM_DRP_ENUMERATOR_NAME;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Enumerator name: ";
	};
	struct DEVICEDESC :bDevCreator<DEVICEDESC> {
		static const T ID = 17;
		static const T CM_DRP = CM_DRP_DEVICEDESC;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Description: ";
	};
	struct FRIENDLYNAME :bDevCreator<FRIENDLYNAME> {
		static const T ID = 18;
		static const T CM_DRP = CM_DRP_FRIENDLYNAME;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Friendly name: ";
	};
	struct DRIVER :bDevCreator<DRIVER> {
		static const T ID = 19;
		static const T CM_DRP = CM_DRP_DRIVER;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Driver: ";
	};
	struct PHYSICAL_DEVPATH :bDevCreator<PHYSICAL_DEVPATH> {
		static const T ID = 20;
		static const T CM_DRP = CM_DRP_PHYSICAL_DEVICE_OBJECT_NAME;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Physical Object Name: ";
	};
	struct UI_NUMBER :bDevCreator<UI_NUMBER> {
		static const T ID = 21;
		static const T CM_DRP = CM_DRP_UI_NUMBER;
		static constexpr TCHAR FRIENDLY_NAME[100] = "UI number: ";
	};
	struct UPPERFILTERS :bDevCreator<UPPERFILTERS> {
		static const T ID = 22;
		static const T CM_DRP = CM_DRP_UPPERFILTERS;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Upper filters: ";
	};
	struct LOWERFILTERS :bDevCreator<LOWERFILTERS> {
		static const T ID = 23;
		static const T CM_DRP = CM_DRP_LOWERFILTERS;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Lower filters: ";
	};
	struct BUSTYPEGUID :bDevCreator<BUSTYPEGUID> {
		static const T ID = 24;
		static const T CM_DRP = CM_DRP_BUSTYPEGUID;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Bustype GUID: ";
	};
	struct LEGACYBUSTYPE :bDevCreator<LEGACYBUSTYPE> {
		static const T ID = 25;
		static const T CM_DRP = CM_DRP_LEGACYBUSTYPE;
		static constexpr TCHAR FRIENDLY_NAME[100] = "Legacy bus type: ";
	};

	

#undef SUBSTATICCLASSCREATE
#undef SET_DRP
#undef SET_ID
#undef SET_FRIENDLYNAME
};
using TDevID=TDevice_ID<>;

//const TDevID::bDev* TDevID::DEVICEID::Base = TDevID::baseFactory<TDevID::DEVICEID>();
#endif //_DEVICE_DEF_H_

/*struct base1 {base1(const int value) : b(value) {} const int b;};
template <class T = delivered >
inline constexpr base1* rr() {return new base1(T::bb);}
template <class T > struct base2 {static const base1* b;};
struct base3 { static const base1* b; };
struct delivered11 : public base1 {//: public base2<delivered> {
	delivered11():base1(bb){}
	static constexpr int bb = 8;};
struct delivered : public base3 {//: public base2<delivered> {
	//delivered():base2<delivered>(){}
	static constexpr int bb = 8;static const base1* rb;};
const base1* base3::b = rr<delivered>();
const base1* base2<delivered>::b = rr<delivered>();
const base1* delivered::rb = rr<delivered>();
#include "Header.h"
//const float base::b = 3.;//const float delivered::b = 3.1415926535;
//test:://base1* base2::b = nullptr;::b = nullptr;
int main() {auto b = new delivered();
	//delivered::b	//auto jj=b->ttt();
	//auto uiii=jj.b;auto uuu = b->b->b;
	auto uuu2 = delivered::rb->b;
	//b = d;//float hh =dynamic_cast<delivered&>(b).b;
	std::cout << "Hello World!\n";
}*/