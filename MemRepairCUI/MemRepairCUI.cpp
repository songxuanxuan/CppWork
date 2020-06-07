#include<Windows.h>
#include "MemSearch.h"
#include <stdio.h>


int main()
{
	WCHAR szFileName[] = L"E:\\TestApp.exe";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if(!::CreateProcess(NULL, (LPWSTR)szFileName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		printf("进程创建失败    错误码：%d \n", GetLastError());
	::CloseHandle(pi.hThread);

	MemSearch ms;
	ms.SetG_hProcess(pi.hProcess);

	//输入修改值
	int iVal;
	printf("输入搜索值: \n");
	scanf("%d", &iVal);

	ms.FindFirst(iVal);
	ms.ShowList();

	::CloseHandle(pi.hProcess);
	return 0;

}