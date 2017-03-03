// dllExercise.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return true;
}

extern "C" _declspec(dllexport)int max(int _a,int _b)
{
	if(_a>_b)
		return _a;
	else
		return _b;
}

extern "C" _declspec(dllexport)int min(int _a,int _b)
{
	if(_a<_b)
		return _a;
	else
		return _b;
}

extern "C" _declspec(dllexport)int add(int _a,int _b)
{
	return _a+_b;
}
