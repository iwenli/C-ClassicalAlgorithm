// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//


#if !defined(AFX_STDAFX_H__E431958F_70E8_4BE4_A04F_1288E3CCDBD5__INCLUDED_)
#define AFX_STDAFX_H__E431958F_70E8_4BE4_A04F_1288E3CCDBD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
extern "C" _declspec(dllexport) int max(int _a,int _b);
extern "C" _declspec(dllexport) int min(int _a,int _b)
extern "C" _declspec(dllexport) int add(int _a,int _b);
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E431958F_70E8_4BE4_A04F_1288E3CCDBD5__INCLUDED_)
