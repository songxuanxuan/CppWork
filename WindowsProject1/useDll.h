#pragma once
#include <stdio.h>
#pragma comment(lib, "dllexample.lib")


_declspec(dllimport) int add(int a, int b);
_declspec(dllimport) int sub(int a, int b);
void TestDll();
