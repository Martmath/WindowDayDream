#pragma once
//#include "stdafx.h"
#include <iostream>
#include <vector>
#include "resource.h"
#include <string>  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define AA( x ) (((x) < 5) ? (6) : (3))
#define YY( x ) (x) < 5) ? "16" : "13"
#define SS(x) "0x"
#define ZZ( x ) SS(x)##YY(x)
#define CC( x ) (((x) == 0000) ? (0) : ((x) == 0001) ? (1) : ((x) == 0010) ? (2) : \
((x) == 0011) ? (3) : ((x) == 0100) ? (4) : ((x) == 0101) ? (5) :((x) == 0110) ? (6) : \
((x) == 0111) ? (7) : ((x) == 1000) ? (8) : ((x) == 1001) ? (9) :((x) == 1010) ? (10) : \
((x) == 1011) ? (11) : ((x) == 1100) ? (12) : ((x) == 1101) ? (13) :((x) == 1110) ? (14) : (15))
#define K( x ) ({unsigned short retval; \
switch (x) {case 0000: retval=0; break; case 0001: retval=1; break;\
case 0010: retval=2; break; case 0011:retval=3; break;\
case 0100: retval=4; break; case 0101:retval=5; break;\
case 0110: retval=6; break; case 0111:retval=7; break;\
case 1000: retval=8; break; case 1001:retval=9; break;\
case 1010: retval=10; break; case 1011:retval=11; break;\
case 1100: retval=12; break; case 1101:retval=13; break;\
case 1110: retval=14; break; default:retval = 15;} retval;})//#define F( x ) (do{unsigned short retval=9; retval;} while(0);)
#define F(x) ({ unsigned short nl=9; nl; };)
#define WW( a,b,c,d ) (m( a )<<12)|(m( b )<<8)|(m( c )<<4)|m(d)//#define WW( a,b,c,d ) (CC( a )<<12)|(CC( b )<<8)|(CC( c )<<4)|CC(d)
#define SS( a ) WW(a/1000000000000, (a%1000000000000)/100000000 ,(a%100000000)/10000,a%10000)
inline unsigned short m(unsigned short a) {	switch (a) {
	case 0000: return 0; case 0001: return 1; case 0010: return 2; case 0011: return 3;
	case 0100: return 4; case 0101: return 5; case 0110: return 6; case 0111: return 7;
	case 1000: return 8; case 1001: return 9; case 1010: return 10; case 1011: return 11;
	case 1100: return 12; case 1101: return 13; case 1110: return 14; default:return 15;} return 0; }

inline unsigned short MM(unsigned long long* a) {
	return (m(*a / 1000000000000) << 12) | (m((*a % 1000000000000) / 100000000) << 8) | (m((*a % 100000000) / 10000) << 4) | m(*a % 10000);}

int main3() {unsigned short tr4r = (CC(1101))<< 4; //unsigned short trr = WW(1111, 1111, 1101, 1010);
	/*unsigned short trre = SS(1111111111011010); const char* er = ZZ(14);int dder = YY(14);*/return 0;}

inline int BB(int x) { return ((x) < 5) ? (10) : (15); }//#define BB( x ) if((x) < 5) { (10) } else { (15) };
using namespace std;

template< typename OStreamable > wstring to_string(const OStreamable& o)
{wostringstream w;	w << o;	if (!w) throw "dammit!";return w.str();}
#define to_wstr(a) (to_wstring(a).c_str())
#define sv(T,t) (is_same<T, t>::value)
#define sTv(t) (sv(T, t))
template<typename T> constexpr bool refT_v = sTv(int) || sTv(short) || sTv(char) || sTv(unsigned char);
template<typename T> constexpr bool valT_v = sTv(CString);
template<bool, bool, typename T> struct en_if { };
template<typename T> struct en_if <true, false, T> {using type = T; };
template<typename T> struct en_if <false, true, T> { using type = T*; };
template<bool V, bool P, typename T> using if_t = typename en_if<V, P, T>::type;
size_t def_l;
#define s_0 _T("0")
#define s_1 _T("1")
#define s_ _T("")

/*void foo()
{
#define ndx(a) ((TClass*)(pSomeList->Items[a]))->x
#undef ndx
}*/




#if 1//_MSVC_LANG >= 201700L//C17//(strstr(typeid(U).name(),"std::basic_string<wchar_t"))tuple<R*,int>to_wstring(bs[i])
template <class R = int, class T=char, class U = if_t<valT_v<R>, refT_v<R>, R>>
U get_bits(T n, size_t& l = def_l, CString delim = _T("|")) { U h; size_t L, i;
	if constexpr(valT_v<R>) {char* bs = get_bits<char, T>(n, L); l = L--;
	for (i = L; i > 0; i--) { h += bs[i] ? s_1 : s_0; if (!(i % 8)) h += delim;}h +=bs[0] ? s_1 : s_0; delete(bs);} 
	else { L = l = (sizeof(T) * 8); h =(U)malloc(sizeof(R)*L); constexpr T _1 = 1;
	for (i = 0; i < L; i++)	h[i] = (n & (_1 << i)) != 0;} return h;}

template <class R = int, class T = char, class U = if_t<valT_v<R>, refT_v<R>, R>>
U get_bits_(T n, size_t& l = def_l, CString delim = _T("|")) {U h; size_t L, i;				
if constexpr(valT_v<R>) {
	char* B = get_bits<char, T>(n, L); l = L; h = B[0] ? s_1 : s_0;
	for (i = 1; i < L; i++) { if (!(i % 8)) h += delim; h += B[i] ? s_1 : s_0;} delete(B);}else{
	L = l = (sizeof(T) * 8); h = (U)malloc(sizeof(R)*L); constexpr T _1 = 1;
	for (i = 0; i < L; i++)  h[i] = (n & (_1 << i)) != 0;} return h;}

template <class R = int, class T = char, class U = if_t<valT_v<R>, refT_v<R>, R>>
U get_bits_(T* n, size_t C, size_t& l = def_l, CString delim = _T("|"), CString delim1 = _T("#")) {
	size_t L, k, j, i, N = sizeof(T) * 8; U h; if constexpr(valT_v<R>)
	{char * bs = get_bits_<char, T>(n, C, L, delim, delim1); l = L; h = bs[0] ? s_1 : s_0; for (i = 1; i < L; i++)
		{if (!(i % 8)) h += i%N ? delim : delim1; h += bs[i] ? s_1 : s_0;}delete(bs);}else {
		L = l = N*C; constexpr T _1 = 1; h = (U)malloc(sizeof(R) * L); for (i = 0; i < C; i++)
		{k = i*N; for (j = 0; j < N; j++) h[k + j] = (n[i] & (_1 << j)) != 0;}} return h;}

		
template <class R = int, class T = char, class U = if_t<valT_v<R>, refT_v<R>, R>>
U get_bits(T* n, size_t C, size_t& l = def_l, CString delim = _T("|"), CString delim1 = _T("#")) {
	size_t i, N = sizeof(T) * 8, L = l = N*C; U h; if constexpr(valT_v<R>) {
		h = get_bits<CString, T>(n[0], L, delim); 
		for (i = 1; i < C; i++) {
			auto rr = CString(std::to_string(i).c_str());
			h += delim1 + _T("(") + rr + _T(")") + get_bits<CString, T>(n[i], L, delim);
		}
	}else {
		constexpr T _1 = 1; size_t k, j; h =(U)malloc(sizeof(R) * L); for (i = 0; i < C; i++)
		{ k = i*N; for (j = 0; j < N; j++) h[k + j] = (n[i] & (_1 << j)) != 0;}	} return h;}

template <class R = int, class T = char, class U = if_t<valT_v<R>, refT_v<R>, R>>
U get_bits1(T* n, size_t C, size_t& l = def_l, CString delim = _T("|"), CString delim1 = _T("#")) {
size_t k, j, i, L, N = sizeof(T) * 8; U h;
 if constexpr(valT_v<R>) {char* bs = get_bits1<char, T>(n, C, L, delim, delim1); l = L;
 for (i = 1; i <= C; i++) { k = i*N - 1; h += bs[k] ? s_1 : s_0;
 for (j = 1; j < N; j++) { if (!(i % 8)) h += delim; h += bs[k - j] ? s_1 : s_0; }
	 if (i < C) h += delim1; } delete(bs); } else {
L = l = N*C; constexpr T _1 = 1; h = (U)malloc(sizeof(R) * L);  for (i = 0; i < C; i++)
{ k = i*N; for (j = 0; j < N; j++) h[k + j] = (n[i] & (_1 << j)) != 0;} } return h;}

template <class Q = int, class T> 
Q slice1(T* n, int b, int e) {
	Q R = 0; Q r = 0; Q xx = 1; e--;
	for (int i = 0; i <= (e - b); i++) {r = n[e-i] ? (xx << i) : 0; R = R|r;}return R;}
//char* getBits(T* n, int b, int e) {
//	return 
//}

template <class Q = int, class T> Q slice(T* n, int b, int e) {
	Q R = 0; Q r = 0; Q xx = 1;
	for (int i = 0; i < (e - b); i++) { r = n[i + b] ? (xx << i) : 0; R = R | r; } return R;
}
#else
template <typename R = wstring, typename T = char> enable_if_t<valT_v<R>, R>
	get_bits(T n, int& l = def_l) { //MessageBox(0, to_wstr(_MSVC_LANG),L"",MB_OK);
		wstring h; int L; int* bs = get_bits<int, T>(n, L);
		for (int i = 0; i < L; i++) h = h + ((bs[i] == 0) ? L"0" ? L"1");//to_wstring{bs[i])
		delete(bs); l = L; return h;}
template <typename R = int, typename T = char> enable_if_t<refT_v<R>, R*>
	get_bits(T n, int& l = def_l) {
		int L = sizeof(T) * 8; R* bs = (R*)malloc(sizeof(R)*L);
		for (int i = 0; i < L; i++) bs[i] = n & (1 << i)) != 0; i = L; return bs;}

template <typename R = int, typename T = char> enable_if_t<refT_v<R>, R*>
	get_bits(T* n, int C, int& l = def_l) {
		int k, L = sizeof(T) * 8;
		R* bs = (R*)malloc(sizeof(R) * L * C);
		for (int i = 0; i < C; i++) {
			k = i*L; for (int j = 0; j < L; j++) bs[k + j] = (n[i] & (1 << j)) != 0;
		}l = L*C; return bs;
	}

template <typename R = wstring, typename T = char> enable_if_t<valT_v<R>, R>
	get_bits(T* n, int C, int& l = def_l) {
		int L; R h;  int* bs = get_bits<int, T>(n, C, L); for (int i = 0; i < L; i++) h = h + ((bs[i] == 0) ? L"0" ? L"1");
			delete(bs); l = L; return h;} 
#endif

wstring s2ws(const string& s){int N = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
			 wstring h(N, 0); MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &h[0], N);return h;}

			template <typename T> T Reverse(T _x) {	T Rez = 0;
				for (int i = 0; i < sizeof(T) * 8; i++)
				{Rez = Rez << 1; Rez |= (_x & 1); _x = _x >> 1;} return Rez;}


			template <typename T> T* Reverse(T *n, int C) {
			T* h = (T*)malloc(sizeof(T)*C); 
			for (int i = 0; i < C; i++) h[i] = Reverse<T>(n[i]); return h;}
		template <typename T> T* ReverseAll(T *n, int C) { 
			T* h = (T*)malloc(sizeof(T)*C); int c = C - 1;
			for (int i = 0; i < C; i++) h[i] = Reverse(n[c - i]); return h;}
		template <typename T> T* ReverseBT(T *n, int C) {
			T* h = (T*)malloc(sizeof(T)*C); int c = C - 1;
			for (int i = 0; i < C; i++) h[i] = n[c - i]; return h;}

		template <typename T>enable_if_t<is_same_v<T, string>, string> str(T t) {return t;}
		template <typename T>enable_if_t<!is_same_v<T, string>, string> str(T t) { return to_string(t); }
		void test(){
				unsigned char a[20] = { 47,204,33,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
				//https://habr.com/post/282301/
				 //char vx = 7; vx = (vx << 1);
				 //string eel = "11111";string ee = str(ee1);
				  //int rr = str1(2); int* rr3 = str1(2);
				  //auto hhi = get_bits<int,int>((-10));
				  //auto hhid2 = get_bits<CString,int>((-3072));
				  //auto hhid2f = get_bits<CString, short>((-3072));
				  auto hhi2 = get_bits<CString>(Reverse(-10));
					int bb = 29; time_t start, end; double seconds, sssl; time(&start);
					CString hhid2 = get_bits<CString>(&a[0], 20, def_l);
					time(&end); seconds = difftime(end, start);
					time(&start); CString hhid2__; clock_t start1 = clock();
					//for (int i = 0; i < 64000; i++) {
					//	a[0] = i;
					//	hhid2__ = get_bits<CString>(&a[0], 3, def_l);
					//}
					//clock_t endd = clock(); time(&end); sss1 = difftime(end, start); double seconrds = (double)(endd - start1);
					//					CString hhid2_ = get_bits<CString>(jj); CString hhid2 = get_bits<CString>(&a[0], 2, def_l);
					
					
					//char* Q = get_bits<char>(&a[0], 20);
					//int W = slice(Q, 2, 14);
					//short jj = m0(&a, 2, 14);
					//this->SetDlgItemText(IDC_EDIT1, get_bitsl<CString>(&a[0], 3).GetString()); 
					//this->SetDlgItemText(IDC_EDIT2, get_bits<CString>(W).GetString());
					//this->SetDlgItemText(IDC_EDIT3, get_bits_<CString>(&a[0], 3).GetString());
					//this->SetDlgItemText(IDC_EDIT4, get_bits<CString>(jj).GetString());
return;

					//auto rtt=get_bitsx <int>((-9));auto hhu=get_bitsxS<int>((-9));
					//wstring *hhid = get_bitsSS<int>((-9));wstring hhidi = *hhid;
					// wstring* hhdid = get_bits<char, wstring>(Reverse<char>((-9)));
					//unsigned short jj =m0(&g, 20, 24);main3();int er = AA(0010); int ee = CC(0001); 
					int r = 0; //double seconds;
					//time_t start, end; time(&start); unsigned long long u = 0;
				//	for (int i = 0; i < 10000; i++) {
					//	for (int j = 0; j < 1000; j++)
					//	{
					//		u = (j - i)*(i*j); u = u - (u << 2); r = SS(u);	}	}
				//	time(&end); seconds = difftime(end, start);
				//	printf("Thefge: %f \n", r); printf("The time: %f seconds\n", seconds);
					//int y; //BB(6);
				/*	unsigned short n = (__int8)g[0];//unsigned char
					unsigned short nn = ((n<<4)&0xF00)|g[1];//unsigned char
					n = nn;//1100001
					unsigned char N = 'b';
					//unsigned short nn = N; //__int8 1100010
					int uu = int(n >> 15 & 1); LPCWSTR t = L"1"; 
					if (uu = 0) t = L"0";	OutputDebugString(t);
					//char uu = n >> 6 & 1;
					uu = int(n >> 14 & 1);	t = L"1";
					if (uu = 0) t = L"0";	OutputDebugString(t);
					uu = int(n >> 13 & 1); t = L"l";
					if (uu = 0) t = L"0";	OutputDebugString(t);
					for (int i = 7; i >= 0;) cout << int(n >> i-- & 1) << " "; cout << endl;
					//this->SetDlgItemTextW(IDC_EDIT1, (LPCTSTR)z.c_str()); 
					unsigned char byte = 49; BYTE fff = Reverse(byte);
					//char* bits = m1111(fff); //char bitss[8];

*/
		}