#include "stdafx.h"

BOOL CreateChidProcess(PTCHAR szChidProcessName, PTCHAR szCommandLine)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if (!CreateProcess(
		szChidProcessName,
		szCommandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,  //suspended һ����ķ�ʽ�������߳��ں˶���ETHREAD������͹��𣬻�û��ӳ��dll�ʹ����̡߳�����ҪresumeThread�ָ���
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		printf("error createProcess : %d \n", GetLastError());
		return FALSE;
	}
	printf("process pid: %x    ��� : %x \n", pi.dwProcessId, pi.hProcess);

	for (int i = 0; i < 5; i++)
	{
		Sleep(1000);
		printf("------------ \n");
	}
	ResumeThread(pi.hThread);			//�ָ�

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}

int main1safaf()
{
	WCHAR commandLine[] = L"notepad";
 	CreateChidProcess(NULL, commandLine);

	return 0;
	//HANDLE hProcess;

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, )
}