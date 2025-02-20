//copyStr.cpp
#include "copyStr.h"

void copyStr(char* a, const char* b, int len)
{
	for (int i = 0; i < len; i++)
	{
		a[i] = b[i];
	}
	a[len] = '\0';
}