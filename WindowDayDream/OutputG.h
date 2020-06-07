#pragma once
#include <string>
#include <windows.h>
#include <math.h>

#define EXPAND( x ) x

//http://www.thevalvepage.com/swmonkey/2013/01/30/unicode-and-__file__
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WIDEFILE__ WIDEN(__FILE__)
#define __WIDEFUNCTION__ WIDEN(__FUNCTION__)

//https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros

#if defined(_CONSOLE) || defined(__CONSOLE__)
#define _L //work just inside macros
#define _trace(...)  printf(__VA_ARGS__)
#else
#define _L L //work just inside macros
#define _trace(...)  mytrace(_T(__VA_ARGS__))  
#endif


/*#define IfPrintLINE1(x) if (x) _trace( _L#x  " : condition is not satisfied in line %d of func %s; \r\n", __LINE__, __WIDEFUNCTION__);
#define IfPrintLINE2(x,...) if (x) {_trace(_L##__VA_ARGS__); _trace( "   ;" _L#x  " : condition is not satisfied in line %d of func %s; \r\n", __LINE__, __WIDEFUNCTION__);}
#define GET_LAST_ARG(arg1, arg2, arg3, arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,argN,...) argN
#define PRINT_MACRO_CHOOSER(...)   EXPAND(GET_LAST_ARG(__VA_ARGS__, \
IfPrintLINE2, IfPrintLINE2, IfPrintLINE2, IfPrintLINE2,IfPrintLINE2, IfPrintLINE2, IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE1) (__VA_ARGS__))
#define IfPrintLINE(...)   PRINT_MACRO_CHOOSER(__VA_ARGS__) */

#define GET_LAST_ARG(_gla1, _gla2, _gla3, _gla4, _gla5, _gla6, _gla7, _gla8, _gla9, _gla10, _gla11, _glaN,...) _glaN
#define PRINT_MACRO_CHOOSER(_pmc1, _pmc2, _pmc3, _pmc4,_pmc5, _pmc6, _pmc7,_pmc8,_pmc9,_pmc10,_pmc11,...)   EXPAND(GET_LAST_ARG(__VA_ARGS__, \
_pmc11, _pmc10, _pmc9, _pmc8,_pmc7, _pmc6, _pmc5,_pmc4,_pmc3,_pmc2,_pmc1) (__VA_ARGS__)) 


#define IfPrintLINE1(x) if (x) _trace(#x  " : condition is not satisfied in line %d of func %s; \r\n", __LINE__, __WIDEFUNCTION__);
#define IfPrintLINE2(x,...) if (x) {_trace(__VA_ARGS__); _trace( " ; " #x  " : condition is not satisfied in line %d of func %s; \r\n", __LINE__, __WIDEFUNCTION__);}
#define IfPrintLINE(...)   PRINT_MACRO_CHOOSER(IfPrintLINE1,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2, \
 IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,IfPrintLINE2,__VA_ARGS__) 


#define retLINE return __LINE__;
#define IfretLINE(x) if (x) return __LINE__;


#define DBOUT( s )  { \
   std::ostringstream os_; os_ << s; OutputDebugString( os_.str().c_str() ); \
}

inline bool mytrace(TCHAR *format, ...)
{
	TCHAR buffer[1000];
	va_list argptr;
	va_start(argptr, format);
	wvsprintf(buffer, format, argptr);
	va_end(argptr);
	OutputDebugString(buffer);
	return true;
}

