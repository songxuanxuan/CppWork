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

	printf("ThreadProc_1执行了 \n");

	getchar();
	return 0;
}
DWORD WINAPI ThreadProc_2(LPVOID Parameter)
{
	printf("ThreadProc_2 wait之前执行了 \n");
	WaitForSingleObject(g_hEvent, INFINITE);//如果设置互斥，则在此处等待.修改标志位

	printf("ThreadProc_2执行了 \n");

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
		printf("生产者%d将产品%d放入缓冲区 \n", id, g_Number);
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
		printf("---消费者%d将产品%d放入缓冲区 \n", id, g_Number);
		//ReleaseMutex(hMutex);
		SetEvent(g_hSet);

	}
	return 0;
}
int mainjjjjsl()
{	//参数：安全属性，事件之间是否可以都执行，初始是否有信号，名字
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