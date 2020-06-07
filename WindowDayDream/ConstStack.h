#pragma once
template<typename T, int DATA_COUNT = 30>
struct ConstStack {
	T* data = new T[DATA_COUNT]{ T() };
	static constexpr int sizeMem = sizeof(T) * (DATA_COUNT - 1);
	//push_front
	void add(const T& t) {
		memmove(&data[1], &data[0], sizeMem);
		data[0] = t;
	}
	int i;
	template<class Q>
	void forEach(Q& q, void (*f)(Q& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++)	f(q, data[i]);
	}

	void forEachBeforeTrue(bool& q, void (*f)(bool& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++) {
			f(q, data[i]);
			if (q) return;
		}
	}
	void forEachBeforeFalse(bool& q, void (*f)(bool& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++) {
			f(q, data[i]);
			if (!q) return;
		}
	}
	T* operator[](int n) { return &(data[n]); }
	~ConstStack() { delete[] data; }
};


template<typename T, int DATA_COUNT = 30>
struct ConstStack2 {
	static constexpr int DATA_COUNT_1 = DATA_COUNT - 1;
	T* data = new T[DATA_COUNT]{ T() };
	static constexpr int sizeMem = sizeof(T) * DATA_COUNT_1;
	int c = DATA_COUNT;

	void add(const T& t)
	{
		if (c > 0) c--;  else c = DATA_COUNT_1;
		data[c] = t;

	}

	int i;
	template<class Q>
	void forEach(Q& q, void (*f)(Q& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++)	f(q, data[i]);
	}

	void forEachBeforeTrue(bool& q, void (*f)(bool& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++) {
			f(q, data[i]);
			if (q) return;
		}
	}
	void forEachBeforeFalse(bool& q, void (*f)(bool& out, T& item))
	{
		for (i = 0; i < DATA_COUNT; i++) {
			f(q, data[i]);
			if (!q) return;
		}
	}
	int m;
	T* operator[](int n) {
		m = c + n;
		if (m > DATA_COUNT_1) m -= DATA_COUNT;
		return &(data[m]);
	}
	~ConstStack2() { delete[] data; }
};
