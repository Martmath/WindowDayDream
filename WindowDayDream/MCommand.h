#pragma once
#include <limits>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "MadgwickAHRS\MadgwickAHRS.h"
#include "Triple.h"
#include "Timer.h"
#include "Events.h"
#include "ConstStack.h"
#include "MathBase.h"
#include "ParseByteArray.h"
#include "FileWork.h"
#include "ProjectConsts.h"
#include "DeviceIteraction.h"
using namespace std;


struct DataDevice;

class AGMT {
	 //float& (*value)(AGMT*);
public:
	//float valueResult(){ return value(this); };
	//std::function<float()> valueResult;
	virtual float valueResult()
	{
		complexValue -=
			(*accelerometer - oldAccelerometer) *
			//(1.0f + atan(-2.0f + 1.0f * abs((*(*a).gyroscope) / 100.0f)) - atan(-2.0f));	
			(1.0f + abs((*gyroscope) / 200.0f));
		oldAccelerometer = *accelerometer;
		return complexValue;
	}
	virtual float result()
	{
		return (kDelta - *helperValue) * kMultiply;
	}

	virtual void init(float* a, float* g, float*res= nullptr, float* res2 = nullptr)
	{
		accelerometer = a;
		gyroscope = g;
		if (res == nullptr)
			helperValue = &complexValue;
		else
		helperValue = res;

		helperValue2 = res2;

	}
	
	virtual float result2()
	{
		return -(*helperValue2) * kMultiply;
	}

	float* accelerometer = nullptr;
	float* gyroscope = nullptr;

	float complexValue;

	float* helperValue =  &complexValue;

	float* helperValue2;

	float oldAccelerometer;

	float kDelta = 0;
	float kMultiply = 1;
	//float result;
	friend DataDevice;
};

struct AGMTX :public AGMT{
	float result()
	{
		return (*helperValue-kDelta) * kMultiply;
	}

	float result2()
	{
		return *helperValue2 * kMultiply;
	}
};
struct AGMTY :public AGMT{};

struct AGMTZ :public AGMT
{
	float valueResult()
	{
		complexValue -=
			(*accelerometer - oldAccelerometer) *
			(1.0f + atan(-2.0f + 1.0f * abs((*gyroscope) / 100.0f)) - atan(-2.0f));
		//(1.0f + abs((*gyroscope) / 200.0f));
		oldAccelerometer = *accelerometer;
		return complexValue;
	}

	float result()
	{
		return (kDelta - *helperValue) * kMultiply;
	}	
};

struct DataAGMT {

	//ConstStack<float, 400> agfmt;

	void init(TripleXYZ<float>& tr)//(const DataAGMT& agmt)
	{
		tr.x = *xAcross.helperValue;
		tr.y = *yAlong.helperValue;
		tr.z = *zFlat.helperValue;
		//return *this;
	}

	bool isSame(float& x,float& y,float& z)
	{
		return (abs(x - *xAcross.helperValue) < 1000) && (abs(y - *yAlong.helperValue) < 1000) && (abs(z - *zFlat.helperValue) < 1000);
	}
	bool isSame(TripleXYZ<float>& xyz)
	{
		return (abs(xyz.x - *xAcross.helperValue) < 1000) && (abs(xyz.y - *yAlong.helperValue) < 1000) && (abs(xyz.z - *zFlat.helperValue) < 1000);
	}

	AGMTY yAlong;//roll
	AGMTX xAcross;//pitch
	AGMTZ zFlat;//yaw
	short xTouch = smallNumMax;//9
	short yTouch = smallNumMax;//10
	TripleXYZ<float> madgwickXYZ;

};
struct DataButtons {
	bool bSoundPlus = false;/*11*/
	bool bSoundMinus = false;/*12*/
	bool bNearTouch = false;/*13*/
	bool bNearRun = false;/*14*/
	bool bTouch = false;/*15*/
};

enum enumRunMode { nothing = 0, mouseRM, catiaRM };
struct TscData
{
	TripleXYZ<float> bXYZ;
	timer aTimer;

	TripleXYZ<double> oXYZ{ 0,0,0 };
	TripleXYZ<double> nXYZ{ 0,0,0 };
	TripleXYZ<double> dXYZ{ 0,0,0 };

	int ff = 0;
	bool ini = false;
	float bX = 0.0;
	float bY = 0.0;

	void (*runMoveTmp)() = 0;
	void (*runMoveTouchTmp)() = 0;

	enumRunMode runMode = nothing;
};

template<class T, class... TT>
struct Checkable
{
	static constexpr void check()
	{
		T::check();
		if constexpr (sizeof...(TT) > 0) Checkable<TT...>::check();
	}
};

struct DataDevice
{
	PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters;
	
	unsigned char(*bitData)[20];

	DataAGMT agmt;
	DataButtons b;

	TscData eventData;
	TButtonCheck bTouch = TButtonCheck(&(b.bTouch), "bTouch");
	TButtonCheck bNRun = TButtonCheck(&(b.bNearRun), "bNRun");
	TButtonCheck bNTouch = TButtonCheck(&(b.bNearTouch), "bNTouch");
	TButtonCheck bSoundPlus = TButtonCheck(&(b.bSoundPlus), "bSoundPlus");
	TButtonCheck bSoundMinus = TButtonCheck(&(b.bSoundMinus), "bSoundMinus");

	TAGMCheck bagmt = TAGMCheck(*agmt.xAcross.helperValue, *agmt.yAlong.helperValue, *agmt.zFlat.helperValue, "moveAGM");

	TTouchCheck btouch = TTouchCheck(&(agmt.xTouch), &(agmt.yTouch), "moveTouch");


	 std::tuple<TButtonCheck*, TButtonCheck*, TButtonCheck*, TAGMCheck*, TTouchCheck*, TButtonCheck*, TButtonCheck*> tp
	 { &bTouch, &bNRun, &bNTouch, &bagmt,&btouch,&bSoundPlus,&bSoundMinus };	

	DataDevice(int N)
	{		
		agmt.xAcross.init(&accel.x, &gir.x, &accel.x, &accelData.x);
		agmt.yAlong.init(&accel.y, &gir.y, &accel.y, &accelData.y);
		agmt.zFlat.init(&agmt.madgwickXYZ.z, &gir.z, &agmt.madgwickXYZ.z, &agmt.madgwickXYZ.z);			
		if (loadFrom(N, agmt.xAcross.kDelta, agmt.yAlong.kDelta, agmt.zFlat.kDelta,
			agmt.xAcross.kMultiply, agmt.yAlong.kMultiply, agmt.zFlat.kMultiply))	
			calibrate = calibrateN::CALIBRATE_BYFILE;		
	}


	TripleXYZ<float> gir;
	TripleXYZ<float> accel;

	bool ini = false;
	bool iniX = false;
	bool iniY = false;
	TripleXYZ<float> accelOld;
	TripleXYZ<float> accelData;	

	Madgwick mM = Madgwick();


	TripleXYZ<float> magn;//расходимся нас обманули - це какие-то левые данные, а не магнитик

	calibrateN calibrate = calibrateN::NO_CALIBRATE;
	scenarioN scenario = scenarioN::NO_ACTION;
	
	ConstStack<TripleXYZ<float>, 3> gOld;
	bool bX = false;
	bool bY = false;
	bool bZ = false;

	float bX_ = 0;
	float bY_ = 0;






	static constexpr auto lx = [](bool& r, TripleXYZ<float>& t) { r = r || (abs(t.x) < 10); };
	static constexpr auto ly = [](bool& r, TripleXYZ<float>& t) { r = r || (abs(t.y) < 20); };
	static constexpr auto lz = [](bool& r, TripleXYZ<float>& t) { r = r || (abs(t.z) < 10); };

	static constexpr auto lx_ = [](float& r, TripleXYZ<float>& t) { r = r + sign(t.x) ; };
	static constexpr auto ly_ = [](float& r, TripleXYZ<float>& t) { r = r + sign(t.y) ; };
	int cycleCount = 0;


	void init()
	{
		gir.x = getDataBytes<6, 16>(bitData); gir.y = getDataBytes<8, 16>(bitData); gir.z = getDataBytes<7, 16>(bitData);
		gOld.add(gir);

		//auto ww = [](bool& r, TripleXYZ<float>& t) { r = r && (abs(t.x) > 20); };
		//gOld.forEach<bool>(rr, ww);


		bX = false; bY = false; bZ = false;
		bX_ = 0; bY_ = 0;
		gOld.forEachBeforeTrue(bX, lx);
		gOld.forEachBeforeTrue(bY, ly);
		gOld.forEachBeforeTrue(bZ, lz);

		gOld.forEach<float>(bX_, lx_);
		gOld.forEach<float>(bY_, ly_);


#ifdef MADGWICK_CHECK
		magn.x = getDataBytes<0>(bitData);
		accel.x = getDataBytes<5>(bitData);
		accelData.x = accel.x;
		agmt.xAcross.valueResult();

		magn.y = getDataBytes<2>(bitData);
		accel.y = getDataBytes<3>(bitData);
		accelData.y = accel.y;
		agmt.yAlong.valueResult();

		magn.z = getDataBytes<1, 8>(bitData);
		accel.z = getDataBytes<4>(bitData);
		//gZ = gir.z;
		mM.updateIMU(gir.x, gir.y, gir.z, accel.x, accel.y, accel.z);
		agmt.madgwickXYZ.Init(roundBase(mM.getPitch() * 100, 5.0f) / 2,
			roundBase((mM.getRoll() * 100), 5.0f) / 10, roundBase(mM.getYaw() * 100, 5.0f));
		agmt.zFlat.valueResult();

#else

		if (!bX)
		{
			magn.x = getDataBytes<0>(bitData);
			accel.x = getDataBytes<5>(bitData);

			if (!iniX)
			{
				accelData.x = accel.x;
				iniX = true;
			}
			else
			{
				//accelData.x = accelData.x + accel.x - accelOld.x;// (abs(accel.x - accelOld.x) * sign(gir.x));
				accelData.x = accelData.x + (abs(accel.x - accelOld.x) *sign(-bX_));
				/*cycleCount++;					
				if (cycleCount == 60) {
					float k0 = accel.x - accelOld.x;
					auto k1 = -abs(accel.x - accelOld.x);
					auto k2 = accelData.x + -(abs(accel.x - accelOld.x));
					auto k3 = accelData.x + ((accel.x - accelOld.x));
					auto k4  = accelData.x + (abs(accel.x - accelOld.x) * sign(gir.x));
					int stopp = 1;
				}*/

				//float k5 = sign(bY_);
				//int stopp1 = 1;
			}
			accelOld.x = accel.x;
			//accelData.x = accel.x;	
			agmt.xAcross.valueResult();
		}
		else bX = false;

		if (!bY)
		{
			magn.y = getDataBytes<2>(bitData);
			accel.y = getDataBytes<3>(bitData);

			if (!iniY)
			{
				accelData.y = accel.y;
				iniY = true;
			}
			else
			{
				//accelData.y = accelData.y + accel.y - accelOld.y;// (abs(accel.y - accelOld.y) * sign(gir.y));
				accelData.y = accelData.y + (abs(accel.y - accelOld.y) * sign(-bY_));
				float ww = sign(bY_);
				int eww = 9;
			}
			accelOld.y = accel.y;
			//accelData.y = accel.y;	
			agmt.yAlong.valueResult();
		}
		else bY = false;

		if (!bZ)
		{
			magn.z = getDataBytes<1, 8>(bitData);
			accel.z = getDataBytes<4>(bitData);
			//gZ = gir.z;
			mM.updateIMU(gir.x, gir.y, gir.z, accel.x, accel.y, accel.z);
			agmt.madgwickXYZ.Init(roundBase(mM.getPitch() * 100, 5.0f) / 2,
				roundBase((mM.getRoll() * 100), 5.0f) / 10, roundBase(mM.getYaw() * 100, 5.0f));
			agmt.zFlat.valueResult();
		}
		else bZ = false;
#endif
		agmt.xTouch = getData<9>(bitData);// parseByteArray1(g, bitsN[9], bitsN[10]);
		agmt.yTouch = getData<10>(bitData);// parseByteArray1(g, bitsN[10], bitsN[11]);
		b.bSoundPlus = getData<11>(bitData);
		b.bSoundMinus = getData<12>(bitData);
		b.bNearTouch = getData<13>(bitData);
		b.bNearRun = getData<14>(bitData);
		b.bTouch = getData<15>(bitData);

		/*if (!ini) {
			//accelOld = accel;
			accelData = accel;
			ini = true;
			//agmt.xAcross.init(&accel.x, &gir.x, &accel.x);
			//agmt.yAlong.init(&accel.y, &gir.y, &accel.y);			
		}
		else
		{			
			accelData.x = accelData.x + (abs(accel.x - accelOld.x) * sign(-gir.x));
			accelData.y = accelData.y + (abs(accel.y - accelOld.y) * sign(gir.y));
			//accelData.x = accel.x;  
			//accelData.y = accel.y;
		}
		accelOld = accel;
		*/		   		 
	}

	static constexpr auto checkAll = [](auto&... tests) {(tests->check(), ...); };	
	void init2()
	{
		bitData = (unsigned char(*)[20])(ValueChangedEventParameters->CharacteristicValue->Data);
		init();
		std::apply(checkAll, tp);
	};

};

struct DataDeviceList {
	ConstStack<DataAGMT, 400> agmt;
	ConstStack<DataButtons, 200> b;
	void add(const DataDevice& t) {
		agmt.add(t.agmt);
		b.add(t.b);
	}
}static actionPack,actionPack1;


template<class D>
void SomethingHappenedFirst
(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter,	PVOID Context);//, DataDevice* r);

template<class D>
void SomethingHappenedSecond
(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter,	PVOID Context);//, DataDevice* r);


template <int i>
struct Device
{
	using EV = Events<Device<i>>;
	static DataDevice data;
	static  void (*SomethingHappened)
		(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context);
};

template <class T>struct getTDeviceN {
	static constexpr int N = -1;
};

template <int InN>struct getTDeviceN<Device<InN>>
{
	static constexpr int N = InN;
};

using TFirstDevice = Device<0>; DataDevice TFirstDevice::data(getTDeviceN<TFirstDevice>::N);
void (*TFirstDevice::SomethingHappened)(BTH_LE_GATT_EVENT_TYPE, PVOID, PVOID)
= [](BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context) {
	SomethingHappenedFirst<TFirstDevice>(EventType, EventOutParameter, Context);// , & TMouse::data);
};


using TSecondDevice = Device<1>; DataDevice TSecondDevice::data(getTDeviceN<TSecondDevice>::N);//fist time its just show data
void (*TSecondDevice::SomethingHappened)(BTH_LE_GATT_EVENT_TYPE, PVOID, PVOID)
= [](BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context) 
{
	SomethingHappenedSecond<TSecondDevice>(EventType, EventOutParameter, Context);// , & TShow::data);
};


/*
//data.g = (unsigned char(*)[20])(ValueChangedEventParameters->CharacteristicValue->Data);
//data.init();	aa.add(data);	//qw.b = &(data.b.bTouch);
//qw.check();

struct dB {	//deque<dataAGMT,> aDeque
	int k[3][20];	int f[2][20];
	int b1[4];	int b2[4];
} static db;

bool happen(DataDeviceList* aa) {
	bool res = aa->b[0]->bNearTouch;
	int count = 0;
	return false;
}

struct dA {
	int k[3][20];	int f[2][20];
	int b1[4];	int b2[4];
} static da;
deque<int> aDeque;


static constexpr const
int tst(){	return 4;}

//int (*p[4]) (int x, int y);
static constexpr const 
int (*p[1]) () { tst };
struct mCommand {
	const char* name = "test";
	const int funcNum = 0;
	const int(*fp)() = p[funcNum];
//sleep or not
	bool active = true;
	//it happen
	bool happen = false;
	//private:
	const 	mCommand* issleepfrom[2] = { nullptr };
	const 	mCommand* iscombinefrom[2] = { nullptr };
	const bool defActive = true;

	void life()
	{
		for (const mCommand* v : issleepfrom) {
			if ((v != nullptr) && (v->happen)) {
				active = defActive;
				happen = false;
				//break;
				goto end_;
			}
		}
		if (happened(da)) {
			for (const mCommand* v : iscombinefrom) {
				if ((v != nullptr) && (!(v->happen)))
					goto end_;
			}
			happen = true;
			to_do();
		}
	end_:
		return;
	}
	void to_do() {}

	//virtual 
	bool happened(dA DA) const	{return false;	}



	//активна если не спит//
//проверят прислали ей на засыпание или нет слип автоматом обнуляет
//обнуляют или нет 
	//constexpr CM() :
//	issleepfrom{ nullptr ,nullptr },
//iscombinefrom{ nullptr ,nullptr }{};
//знает про разбудимых, знает от кого спит и обнуляется
//0 конст спящая ли по умолчанию
	//const 	CM* issleepfrom =nullptr;
	//const int* array = { 0 };
	//const CM* iscombinefrom = nullptr;
	//1 спит не спит
	//1.1 произошло или ьнет
	//анализ было или нет
	//разбудить разбудимых 
	// послать сигнал на засыпание/обнуление
	// засыпить засыпимых обнулить обнулимых

} static cm1;
//Фантазия на тему одновременного нажатия на несколько кнопок одновременно и/или в определенной очередности -
шизофрения вылечена с помошью самогона и шаурмы, забыл че хотел - удалять жалко
static mCommand* begcmlist[5];
static mCommand* cmlist[5];

*/