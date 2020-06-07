#include <Windows.h>
#include <stdio.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	TCHAR className[] = TEXT("my first GUI");
	WNDCLASS windClass = { 0 };
	windClass.lpszClassName = className;

}