#include <Windows.h>
#include <stdio.h>

HANDLE g_hEvent;
int g_Max = 10;
int g_Number = 0;
HANDLE hMutex;
HANDLE g_hSet;
HANDLE g_hClear;

DWORD WINAPI ThreadProc_1(LPVOID Parameter)
{
	WaitForSingleObject(g_hEvent, INFINITE);

	printf("ThreadProc_1ִ���� \n");

	getchar();
	return 0;
}
DWORD WINAPI ThreadProc_2(LPVOID Parameter)
{
	printf("ThreadProc_2 wait֮ǰִ���� \n");
	WaitForSingleObject(g_hEvent, INFINITE);//������û��⣬���ڴ˴��ȴ�.�޸ı�־λ

	printf("ThreadProc_2ִ���� \n");

	getchar();
	return 0;
}

DWORD WINAPI ThreadProduct(LPVOID Parameter)
{
	for (int i = 0; i < g_Max; i++)
	{
		printf("ThreadProduct \n");
		WaitForSingleObject(g_hSet, INFINITE);
		g_Number = 1;
		DWORD id = GetCurrentThreadId();
		printf("������%d����Ʒ%d���뻺���� \n", id, g_Number);
		SetEvent(g_hClear);
		//ReleaseMutex(hMutex);

	}
	return 0;
}DWORD WINAPI ThreadConsumer(LPVOID Parameter)
{
	for (int i = 0; i < g_Max; i++)
	{
		printf("ThreadConsumer \n");
		WaitForSingleObject(g_hClear, INFINITE);
		g_Number = 0;
		DWORD id = GetCurrentThreadId();
		printf("---������%d����Ʒ%d���뻺���� \n", id, g_Number);
		//ReleaseMutex(hMutex);
		SetEvent(g_hSet);

	}
	return 0;
}
int mainjjjjsl()
{	//��������ȫ���ԣ��¼�֮���Ƿ���Զ�ִ�У���ʼ�Ƿ����źţ�����
	/*g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hThread[2];

	hThread[0] = CreateThread(NULL, 0, ThreadProc_1, NULL, FALSE, NULL);
	hThread[1] = CreateThread(NULL, 0, ThreadProc_2, NULL, FALSE, NULL);

	SetEvent(g_hEvent);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(g_hEvent);*/

	g_hSet = CreateEvent(NULL, FALSE, TRUE, NULL);
	g_hClear = CreateEvent(NULL, FALSE, FALSE, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE hThread[2];

	hThread[0] = CreateThread(NULL, 0, ThreadProduct, NULL, FALSE, NULL);
	hThread[1] = CreateThread(NULL, 0, ThreadConsumer, NULL, FALSE, NULL);

	//SetEvent(g_hEvent);

	WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hMutex);
	CloseHandle(g_hEvent);
	return 0;
}