
#include "myDll.h"
#include "stdio.h"

int max(int _a,int _b)
{
	if(_a>_b)
		return _a;
	else
		return _b;
}

int min(int _a,int _b)
{
	if(_a<_b)
		return _a;
	else
		return _b;
}

int add(int _a,int _b)
{
	return _a+_b;
}