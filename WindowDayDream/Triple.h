#pragma once

template<class T>
struct TripleXYZ
{
	constexpr TripleXYZ<T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
	constexpr TripleXYZ<T>() {}
	
	
	T x{};
	T y{};
	T z{};
	/*TripleXYZ<float>& operator= (const DataAGMT& agmt)
	{
		x = *agmt.xAcross.helperValue;
		y= *agmt.yAlong.helperValue;
		z = *agmt.zFlat.helperValue;
		return *this;
	}*/

	void Init(T&& x_, T&& y_, T&& z_)
	{ x = std::move(x_); y = std::move(y_); z = std::move(z_); };
	
	
	TripleXYZ<T>& operator= (const TripleXYZ<T>& xyz)
	{
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
		return *this;
	}


	template<class Q> constexpr
	TripleXYZ<T>& operator= (const TripleXYZ<Q>& xyz)
	{
		x = static_cast<T>(xyz.x);
		y = static_cast<T>(xyz.y);
		z = static_cast<T>(xyz.z);
		return *this;
	}

	friend TripleXYZ<T> operator + (TripleXYZ<T> const& c1, TripleXYZ<T> const& c2)
	{
		return TripleXYZ<T>{c1.x + c2.x, c1.y + c2.y, c1.z + c2.z};
	}

	friend TripleXYZ<T> operator - (TripleXYZ<T> const& c1, TripleXYZ<T> const& c2)
	{
		return TripleXYZ<T>{c1.x - c2.x, c1.y - c2.y, c1.z - c2.z};
	}

	void add(const TripleXYZ<T>& a, const TripleXYZ<T>& b)
	{
		this->x = a.x + b.x;
		this->y = a.y + b.y;
		this->z = a.z + b.z;
	}

	void subtract(const TripleXYZ<T>& a, const TripleXYZ<T>& b)
	{
		this->x = a.x - b.x;
		this->y = a.y - b.y;
		this->z = a.z - b.z;
	}

	bool allTrue(bool(*f)(const T& item))
	{
		return f(x) && f(y) && f(z);
	}
	bool someTrue(bool(*f)(const T& item))
	{
		return f(x) || f(y) || f(z);
	}
};
