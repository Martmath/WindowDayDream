#pragma once

#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

template<typename OutT, class ...InT>
struct BaseFunctor
{
	uintptr_t fI=0;
	uintptr_t oI=0;
	
	BaseFunctor<OutT, InT...>()  {};
	BaseFunctor<OutT, InT...>(uintptr_t i) : fI{ i } {};
	BaseFunctor<OutT, InT...>(uintptr_t Fi, uintptr_t Oi) : fI{ Fi },oI{Oi} {};

	virtual OutT operator()(InT... x) { throw "override functor!!!"; };//=0;	

	bool operator<(const BaseFunctor& other) const	{	
		return (fI < other.fI )|| (!(other.fI < fI) && oI < other.oI);	}

	bool operator == (BaseFunctor<OutT, InT...>& c1) {
		return (c1.fI == this->fI) && (c1.oI == this->oI);
	}
	bool operator!= (BaseFunctor<OutT, InT...>& c1) { return !(c1 == *this); }

	struct pCompare 
	{
		bool operator () (const BaseFunctor<OutT, InT...>* c1, const BaseFunctor<OutT, InT...>* c2) const
		{
			return (c1->fI < c2->fI) || (!(c2->fI < c1->fI) && c1->oI < c2->oI);
		};
	};
};

template<typename OutT, class ...InT>
struct FuncFunctor:BaseFunctor<OutT, InT...>
{
	
	OutT (*f) (InT...) = NULL;
	FuncFunctor(OutT(*Func)(InT...)) :f{ Func }
	{
		fI = reinterpret_cast<uintptr_t>(f);
	};

	OutT operator()(InT... x) override final { return (*f)(x...); }
};

template<typename TObj, typename OutT, class ...InT>
struct ObjFunctor : BaseFunctor<OutT, InT...> {
	OutT(TObj::* f) (InT...) = NULL;
	TObj* o=0;
	bool insideO = false;

	ObjFunctor(OutT(TObj::* Func)(InT...)) 
	{
		o = new TObj{}; f = Func; insideO = true;
//https://stackoverflow.com/questions/18814722/cast-pointer-to-member-function-to-intptr-t
	auto t = f;     
	fI = *reinterpret_cast<intptr_t*>(&t);
	oI = reinterpret_cast<uintptr_t>(o);
	}
	ObjFunctor(TObj* obj, OutT(TObj::* Func)(InT...))   
	{
		o = obj;
		f = Func;
		auto t = f;       
		fI = *reinterpret_cast<intptr_t*>(&t);
		oI = reinterpret_cast<uintptr_t>(o);
	}
	~ObjFunctor() {
		if (insideO) delete o;
	}
	OutT operator()(InT... x) override final { return ((o)->*f)(x...); }
};

template<typename OutT, class ...InT>
struct fnFunctor : BaseFunctor<OutT, InT...> {
	function<OutT(InT...)> f=NULL;

	fnFunctor(function<OutT(InT...)> Func) :f{ Func }
	{
		fI = reinterpret_cast<uintptr_t>(&f);
	};
	
	OutT operator()(InT... x) override final { return f(x...); }
};



template<typename OutT, class ...InT>
struct DelegateS
{
	std::set<BaseFunctor<OutT, InT...>*, typename BaseFunctor<OutT, InT...>::pCompare> s;

	void add(BaseFunctor<OutT, InT...>* f) { s.insert(f); }

	void iterate() { for (auto& i : s) 	(*i)(); }

	OutT operator()(InT... x) override final { return (*f)(x...); }

};

enum  DelegateC : int {	vector = 0,	site = 1};

template < typename OutT,int r=0, typename...  InT> struct  DelegateV;


template <typename... InT>
struct DelegateV<void, 0, InT...>
{
	std::vector<BaseFunctor<void, InT...>*>	s;
	bool delF = false;//todo replace by vector
	
	//for painless replacement of func pointers 
	DelegateV<void, 0, InT...>& operator = (void(*fin) (InT...)) 
	{
		if (delF) { delete s[0]; delF = false; }
		s.clear();
		s.push_back((new FuncFunctor<void, InT...>(fin)));
		delF = true;
		return *this;
	}
	//for painless replacement of func pointers
	DelegateV<void, 0, InT...>& operator = (int i) 
	{
		if (i < s.size()) 
		{
			if (delF && (i == 0)) { delete s[0]; delF = false; }//todo second bool vector
			s.erase(s.cbegin() + i, s.cend());
		}
		return *this;
	}

	operator bool() const
	{
		return s.size() > 0;
	}

	void Clear() {
		if (delF) {delete s[0]; delF = false;}
		s.clear();
	}

	void Delete(int i) {
		if (delF && (i == 0)) { delete s[0]; delF = false; }
		vec.erase(vec.begin() + i);
	}

	void Delete(BaseFunctor<void, InT...>* i)
	{
		auto position = std::find(s.begin(), s.end(), i);
		if (delF && (position == s.begin())) { delete s[0]; delF = false; s.erase(position)	}
		else if (position != s.end()) s.erase(position);
	}

	void add(BaseFunctor<void, InT...>* f) { s.push_back(f); }

	void operator()() {	for (auto& i : s) (*i)();}

	~DelegateV<void, 0, InT...>() {	if (delF) { delete s[0]; }
	}
};

template < typename OutT, typename... InT>
struct DelegateV<OutT, 0, InT...>
{
	OutT res = OutT{};
	void(*f) (OutT&&, OutT*) = [](OutT&& fRes, OutT* inRes)
	{
		(*inRes) = std::move(fRes);
	};//ww;
	std::vector<BaseFunctor<OutT, InT...>*>	s;

	void add(BaseFunctor<OutT, InT...>* f) { s.push_back(f); }

	OutT operator()() {
		for (auto& i : s) 	f((*i)(), &res);
		return res;
	}
};


template < typename OutT, typename... InT>
struct DelegateV<OutT, 1, InT...>
{
	OutT res = OutT{};
	void(*f) (OutT&&, OutT*) = [](OutT&& fRes, OutT* inRes)
	{
		(*inRes) = std::move(fRes);
	};//ww;
	std::vector<BaseFunctor<OutT, InT...>*>	s;

	void add(BaseFunctor<OutT, InT...>* f) { s.push_back(f); }
	OutT operator()() {
		for (auto& i : s) 	f((*i)(), &res);
		return res;
	}
};

template <typename... InT>
struct DelegateV< void, 1, InT...>
{
	std::vector<BaseFunctor<void, InT...>*>	s;

	void add(BaseFunctor<void, InT...>* f) { s.push_back(f); }
	void operator()() {
		for (auto& i : s) (*i)();

	}
};



static int QQf()
{
	int n = 472;
	cout << "test" << n;
	return n;
}
static int QQf1()
{
	int n = 15;
	cout << "test1" << n;
	return n;
}

static void Q1Qf()
{
	int n = 92;
	cout << "test2" << n;

}
static void Q1Qf1()
{
	int n = 41;
	cout << "test3" << n;

};

struct qgq
{
	int n = 48;
	void ee() {
		n++;	
		cout << "test4" << n;
	}

}static hfgg1;


static void delegateTest() {


	std::function<void()> f_d = Q1Qf1;

	fnFunctor rr(f_d);
	rr();
	qgq hfgg;
	hfgg.n = 7;
	ObjFunctor<qgq, void> reer(&hfgg, &(qgq::ee));

	BaseFunctor<void> wbb = reer;
	DelegateV<void> vv_1;
	vv_1.add(&reer);
	vv_1();
	DelegateV<int, DelegateC::site> vv;
	BaseFunctor<int>& fg = FuncFunctor(QQf);
	BaseFunctor<int>& f1 = FuncFunctor(QQf1);
	vv.add(&f1);
	vv.add(&fg);
	auto wnnj = vv.s.size();
	int wwqq = vv();

	DelegateV<void> vv1;
	BaseFunctor<void>& fg1 = FuncFunctor(Q1Qf);
	BaseFunctor<void>& f11 = FuncFunctor(Q1Qf1);

	vv1();
	vv1.add(&rr);
	vv1();

	//vv1 = Q1Qf;
	//vv1.Delete(vv1.s[0]);
	//vv1.add(&f11);
	//vv1 = 1;
	//vv1.add(&fg1);
	auto wnrnj = vv1.s.size();
	vv1();
	bool eee = fg != f1;



}










/*template<typename OutT, class ...InT>
struct DelegateV
{
	OutT res;
	std::vector<BaseFunctor<OutT, InT...>*>	s;

	void add(BaseFunctor<OutT, InT...>* f) { s.push_back(f); }

	void iterate() { for (auto& i : s) 	(*i)(); };
};*/



//xml doc comment,c++ parameter,keywords, macros, brace maching rectangle



