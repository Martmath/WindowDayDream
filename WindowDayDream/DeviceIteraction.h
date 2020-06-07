#pragma once

#include "ProjectConsts.h"
#include <string>
#include "Delegate.h"
static constexpr int MaxPack = 60;
static constexpr int SmallPack = 35;
static constexpr int ZN = 3;
static constexpr int CountTicktoZero = 40;
//bool f = true;


#define checkLongClick
#define checkHolddown
#define checkMultiTab
constexpr bool _checkMultiTab_ = true;
constexpr bool _checkHolddown_ = true;
constexpr bool _checkLongClick_ = true;


struct TButtonCheck
{
	constexpr
		TButtonCheck(const bool* initiator, const char* name):_initiator(initiator),_name(name) {}
	const
		char* _name = "test";
	const
		bool* _initiator = nullptr;
	//bool res = aa->b[0]->bNearTouch;
	int countClick = 0;
	int countZero = MaxPack;
	const void (*beginClick)() = 0;
	const void (*runHolddownClick)() = 0;

	
#ifdef DELEGATE_CHECK
	DelegateV<void> endClick;
#else
	void (*endClick)() = 0;
#endif
	

	void (*beginBigClick)() = 0;
	void (*beginHolddownClick)() = 0;
	void (*endBigClick)() = 0;
	void (*endHolddownClick)() = 0;
	void (*beginZero)() = 0;
	void (*endBigZero)() = 0;
	void (*endZero)() = 0;

	void _beginClick()
	{
		outEventMessage(_name,"beginClick");
		if (beginClick) beginClick();
	}
	void _runHolddownClick()
	{
		if (runHolddownClick) runHolddownClick();
	}

	void _beginHolddownClick() { outEventMessage(_name,"beginHolddownClick");	if (beginHolddownClick) beginHolddownClick(); }
	void _endHolddownClick() { outEventMessage(_name,"endHolddownClick");	if (endHolddownClick) endHolddownClick(); }
	void _endZero() { outEventMessage(_name,"endZero");	if (endZero) endZero(); }
	void _endBigZero() { outEventMessage(_name,"endBigZero");	if (endBigZero) endBigZero(); }
	void _endClick() { outEventMessage(_name,"endClick");		if (endClick) { endClick(); } }
	void _beginBigClick() { outEventMessage(_name,"beginBigClick");	if (beginBigClick) beginBigClick(); }
	void _endBigClick() { outEventMessage(_name,"endBigClick");	if (endBigClick) endBigClick(); }
	void _beginZero() { outEventMessage(_name,"beginZero"); if (beginZero) beginZero(); }
#ifdef nodef
	template<bool _checkMultiTab> void _endZero_() {};
	template<> void _endZero_<true>() { if (countZero < MaxPack) _endZero(); else _endBigZero(); };
	template<> void _endZero_<false>() { _endZero(); };
	void __endZero() { _endZero_<_checkMultiTab_>(); };

	void __runHolddownClick() { if (_checkHolddown_) _runHolddownClick(); };
	void __beginBigClick() { if (_checkLongClick_) _beginBigClick(); };
	void __beginHolddownClick() { if (_checkHolddown_) _beginHolddownClick(); };

	template<bool _checkLongClick, bool _checkHolddown>constexpr void _endClick_() {};
	template<> void _endClick_<true, true>() {
		if (countClick < SmallPack)	_endClick(); else if (countClick < MaxPack)	_endBigClick();	else _endHolddownClick();
	};
	template<> void _endClick_<true, false>() { if (countClick < SmallPack) _endClick(); else  _endBigClick(); };
	template<> void _endClick_<false, true>() { if (countClick < MaxPack) _endClick(); else  _endHolddownClick(); };
	template<> void _endClick_<false, false>() { _endClick(); };
	void __endClick() { _endClick_<_checkLongClick_, _checkHolddown_>(); };
#endif
	static constexpr bool bcheckMultiTab = true;
	static constexpr bool bcheckLongClick = true;
	static constexpr bool bcheckHolddown = true;
	void check()
	{
		if ((*_initiator) || (countZero < ZN))
		{
			//if (countClick < MaxPack)
			//{
			if (!countClick)
			{

				if constexpr (bcheckMultiTab) {
					if (countZero < MaxPack) _endZero(); else _endBigZero();
				}
				else	_endZero();

				_beginClick();
			}//if (countZero) countZero = 0;
			else if (countClick == SmallPack)
			{
				if constexpr (bcheckLongClick) _beginBigClick();
			}
			else if (countClick == MaxPack)
			{
				if constexpr (bcheckHolddown)  _beginHolddownClick();
			}
			else if (countClick > MaxPack) {
				if constexpr (bcheckHolddown) 	_runHolddownClick();
			}
			++countClick;
			//}
			//else if (countClick == MaxPack) 
			//{ 
			//	_runHolddownClick(); 
			//}
			if (!(*_initiator)) ++countZero; else countZero = 0;
		}
		else {
			if (countZero < MaxPack)
			{
				if (countZero == ZN)
				{
					if constexpr (bcheckLongClick && bcheckHolddown)
					{
						if (countClick < SmallPack)
							_endClick();
						else if (countClick < MaxPack)
							_endBigClick();
						else
							_endHolddownClick();
					}
					else
					{
						if constexpr (bcheckLongClick)
						{
							if (countClick < SmallPack) _endClick(); else  _endBigClick();
						}
						else
						{
							if constexpr (bcheckHolddown)
							{
								if (countClick < MaxPack) _endClick(); else  _endHolddownClick();
							}
							else  _endClick();
						}
					}
					_beginZero();
				}
				countClick = 0;
				++countZero;
			}
		}
	}
};
//static bTouch(&(TMouse::data.b.bTouch), "bTouch"), bNRun(&(TMouse::data.b.bNearRun), "bNRun"), bNTouch(&(TMouse::data.b.bNearTouch), "bNTouch"),
//bSoundPlus(&(TMouse::data.b.bSoundPlus), "bSoundPlus"), bSoundMinus(&(TMouse::data.b.bSoundMinus), "bSoundMinus");




struct TTouchCheck
{
	constexpr TTouchCheck(const smallNum* X, const smallNum* Y, const char* name):_X(X), _Y(Y), _name(name)
	{		
	}
	
	const
		char* _name = "test";
	const
		smallNum* _X = nullptr;
	const
		smallNum* _Y = nullptr;

	smallNum Xbeg = smallNumMax;
	smallNum Ybeg = smallNumMax;
	int countTick = 0;
	void (*beginMove)() = 0;
	void (*runMove)() = 0;
	void (*endMove)() = 0;

	void _beginMove() {//outEventMessage(_name,"beginMove");
		if (beginMove) beginMove();
	}
	void _runMove() {// outEventMessage(_name, "runMove"); 
		if (runMove) runMove();
	}
	void _endMove() { //outEventMessage(_name, "endMove"); 
		if (endMove) endMove();
	}
	void check() {
		if ((*_X) | (*_Y))
		{
			if (Xbeg == smallNumMax)
			{
				Xbeg = *_X;
				Ybeg = *_Y;
				_beginMove();
			}
			else { ++countTick; _runMove(); }
		}
		else
		{
			if (Xbeg != smallNumMax)
			{
				_endMove();
				Xbeg = smallNumMax;
				Ybeg = smallNumMax;
			}

		}
	}
};
//static touch(&(TMouse::data.agmt.xTouch), &(TMouse::data.agmt.yTouch), "moveTouch");

static int w = 0;
struct TAGMCheck
{
	constexpr TAGMCheck(const decimal& xAcross, const decimal& yAlong, const decimal& zFlat, const char* name) :
		_xAcross(xAcross), _yAlong(yAlong), _zFlat(zFlat), _name(name)
	{

	}
	const	char* _name = "test";
	const	decimal& const _xAcross = 0;
	const	decimal& _yAlong = 0;
	const	decimal& _zFlat = 0;


	decimal xAcrossPrev = decimalMax;
	decimal yAlongPrev = decimalMax;
	decimal zFlatPrev = decimalMax;
	bool beg = true;
	int countTick = 0;
	int countSameTickEnd = 0;
	int countDiffTickBegin = 0;
	const void (*beginMove)() = 0;
	//const 
	void (*runMove)() = 0;
	const void (*endMove)() = 0;

	void _beginMove() { //outEventMessage(_name, "beginMove"); 
		if (beginMove) beginMove();
	}
	void _runMove() { //outEventMessage(_name, "runMove"); 
		if (runMove) runMove();
	}
	void _endMove() { //outEventMessage(_name, "endMove"); 
		if (endMove) endMove();
	}
	void check() {

		if ((xAcrossPrev != _xAcross) || (yAlongPrev != _yAlong) || (zFlatPrev != _zFlat))
			//if ((xAcrossPrev | yAlongPrev | zFlatPrev) !=((*_xAcross)|(*_yAlong)|(*_zFlat)) )//for int
		{
			//	w++;			if (w == 1800)			{				int rr = 8;			}
			if (beg)
			{
				++countDiffTickBegin;
				if (countDiffTickBegin == 5)// CountTicktoZero)
				{
					_beginMove();
					beg = false;
					countSameTickEnd = 0;
					countTick = 0;
				}
			}
			else { ++countTick; countSameTickEnd = 0;	_runMove(); }
			xAcrossPrev = _xAcross; yAlongPrev = _yAlong; zFlatPrev = _zFlat;
		}
		else
		{
			if (!beg)
			{
				if (countSameTickEnd == 5)// CountTicktoZero)
				{
					_endMove();
					beg = true;
					countSameTickEnd = 0;
				}
				else {
					++countSameTickEnd;
					++countTick;
					_runMove();
				}
			}
			else
			{
				++countSameTickEnd;
				if (countSameTickEnd == 5)// CountTicktoZero)
				{
					countDiffTickBegin = 0;
				}
			}
		}
	}
};
//static agmt(*TMouse::data.agmt.xAcross.helperValue, *TMouse::data.agmt.yAlong.helperValue, *TMouse::data.agmt.zFlat.helperValue, "moveAGM");
//static qwQ(data.gX, data.gY, data.mZ, "moveAGM");
//static qwQ(&(data.agmt.xAcross.value), &(data.agmt.yAlong.value), &(data.agmt.zFlat.value), "moveAGM");

