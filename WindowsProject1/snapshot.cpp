#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include "stdafx.h"

int main123()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	//�����н����Ŀ���
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot����ʧ�� \n");
		return -1;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		printf("�������� %d---------����id  %d \n", pe32.szExeFile, pe32.th32ProcessID);
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	::CloseHandle(hProcessSnap);
	return 0;
}