#include "stdafx.h"

DWORD WINAPI ThreadPlus(LPVOID lpParameter)
{
	int* p = (int*)lpParameter;
	for (int i = 0; i < *p; i++)
	{
		Sleep(1000);
		printf("++++++++ %d\n", i);
	}
	return 0;
}

int main1jsklf ()
{
	/*WCHAR szCommandLine[] = L"cmd";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESIZE|STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	si.dwXSize = 300;
	si.dwYSize = 300;

	BOOL bRet = ::CreateProcess(
		NULL,
		szCommandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);
	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);

		printf("新的进程号： %d \n", pi.dwProcessId);
		printf("新的进程号： %d \n", pi.dwThreadId);
		
	}*/

	HANDLE thread;
	int n = 10;
	thread = CreateThread(NULL, 0, ThreadPlus, &n, 0, NULL);
	CloseHandle(thread);	//内核计数器减一，但是计数器为0后也不会关闭。因为线程要手动关闭才行
	for (int i = 0; i < 7; i++)
	{
		Sleep(1000);
		printf("--------- %d \n", i);
	}

	getchar();
	return 0;
}