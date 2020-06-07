#pragma once
#include "ProjectConsts.h"

constexpr float PI = 3.141592653589793238462643383279502884; /* pi */
constexpr float rad2deg = 180.0 / PI;

template <auto b, typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, T> roundBase(const T& f)
{
	if constexpr (b == BASE_INTEGRAL)
	{
		return f;
	}
	else
	{
		if constexpr (((b & (b - 1)) == 0) && std::is_integral_v<T>)
		{
			constexpr T bb = b - 1; constexpr T bbb = -b;
			return ((f < 0) ? (f + bb) : f) & bbb;
		}
		else
		{
			return round(f / b) * b;
		}
	}
};

template<typename T> constexpr T roundBase(const T& f, const T& b)
{
	if constexpr (std::is_same_v<T, float>)
		return roundf(f / b) * b + 0.0f;
	else
		return round(f / b) * b;
}

inline float toAll(float a)
{
	return 	roundBase(a, 70.0f) + 0.0;
}
inline float toF(short r)
{
	return static_cast<float>(r);
}

template<typename T>
constexpr float toF(T r)
{
	return static_cast<float>(r);
}
template<typename T>
constexpr float toI(T r)
{
	return static_cast<int>(r);
}

float sign(const float& ba) {
	return (ba < 0) ? -1 : 1;    //(ba > 0) - (ba < 0);
}