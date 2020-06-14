#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include "stdafx.h"

int main123()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	//给所有进程拍快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot调用失败 \n");
		return -1;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		printf("进程名称 %d---------进程id  %d \n", pe32.szExeFile, pe32.th32ProcessID);
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	::CloseHandle(hProcessSnap);
	return 0;
}