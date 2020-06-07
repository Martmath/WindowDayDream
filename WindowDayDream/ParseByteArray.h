#pragma once
#include "MathBase.h"
static constexpr int bitsNbeg = 14;
static constexpr int bitsNstep = 13;
static constexpr int bitsN[] =
{ bitsNbeg, bitsNbeg + bitsNstep, bitsNbeg + bitsNstep * 2, bitsNbeg + bitsNstep * 3,
bitsNbeg + bitsNstep * 4, bitsNbeg + bitsNstep * 5, bitsNbeg + bitsNstep * 6,
bitsNbeg + bitsNstep * 7, bitsNbeg + bitsNstep * 8,bitsNbeg + bitsNstep * 9,
139, 147, 148, 149, 150, 151, 152 };
static constexpr int  bitsNsize = sizeof(bitsN) / sizeof(int);

template <int b, int e>
inline short parseByteArray(unsigned char(*a)[20])
{
	const int a0 = b / 8;
	const int a1 = 8 - (b % 8);
	const int l = e - b;
	unsigned
		short r;
	if constexpr (l > a1) {
		if  constexpr (l > (a1 + 8)) {
			int p = l - a1 - 8;
			r = ((*a)[a0] << (16 - a1));
			r = r >> (16 - l);
			r = r + ((*a)[a0 + 1] << (p));
			r = r + ((*a)[a0 + 2] >> (8 - p));
		}
		else {
			r = ((*a)[a0] << (16 - a1));
			r = r >> (16 - l);
			r = r + ((*a)[a0 + 1] >> (8 - l + a1));
		}
	}
	else {
		r = ((*a)[a0] << (8 - a1)) & 255;
		r = r >> (8 - l);
		//if ((l == 1) && (r == 1)) {	wstring tt = to_wstring(a0);}
	}
	if (r > 4096) r = r | 57344;
	//r =r/ 10;
	return *((short*)&r);
}

inline short parseByteArray1(unsigned char(*a)[20], const int b, const int e)
{

	const int a0 = b / 8;
	const int a1 = 8 - (b % 8);
	const int l = e - b;
	unsigned short r;
	if (l > a1) {
		if (l > (a1 + 8)) {
			int p = l - a1 - 8;
			r = ((*a)[a0] << (16 - a1));
			r = r >> (16 - l);
			r = r + ((*a)[a0 + 1] << (p));
			r = r + ((*a)[a0 + 2] >> (8 - p));
		}
		else {
			r = ((*a)[a0] << (16 - a1));
			r = r >> (16 - l);
			r = r + ((*a)[a0 + 1] >> (8 - l + a1));
		}
	}
	else {
		r = ((*a)[a0] << (8 - a1)) & 255;
		r = r >> (8 - l);
		if ((l == 1) && (r == 1)) {
			wstring tt = to_wstring(a0);
		}
	}
	if (r > 4096) r = r | 57344;
	return *((short*)&r);
}

template <int b> inline float getData()
{
	return 	toAll(toF(getData<b>()));
}

template <int b> inline short getData(unsigned char(*bitData)[20])
{
	return 	parseByteArray<bitsN[b], bitsN[b + 1]>(bitData);
}

template <int b, short baseRound = STEP_ROUND> inline float getDataBytes(unsigned char(*bitData)[20])
{
	return 	//toF(gd_<b>()); //gd<b>();//
		static_cast<float>(roundBase<baseRound>(parseByteArray<bitsN[b], bitsN[b + 1]>(bitData)));//       toAll(toF(gd_<b>()));
}
