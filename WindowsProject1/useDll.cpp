#include "stdafx.h"

typedef int (*ipPlus)(int a, int b);
typedef int (*ipSub)(int a, int b);

void TestDll()
{
	int a = add(3, 4);
	int b = sub(4, 3);
	printf("“˛ Ω  add = %d  , sub = %d  \n", a, b);


	HINSTANCE hIns;
	hIns = LoadLibrary(L"dllexample.dll");
	ipPlus myPlus =(ipPlus) GetProcAddress(hIns, "add");
	ipPlus mySub =(ipPlus) GetProcAddress(hIns, "sub");

	a = myPlus(4, 5);
	b = mySub(3, 4);
	printf("œ‘ Ω  add = %d  , sub = %d  \n", a, b);
}