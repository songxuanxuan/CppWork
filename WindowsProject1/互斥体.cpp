#include <Windows.h>
#include <stdio.h>

int mainjkl()
{
	HANDLE g_hMutex = CreateMutex(NULL, true, (LPCWSTR)"xyz");		//true代表本线程是拥有者，把该互斥体占有;;;如果创建失败，返回已经存在的互斥体
	//WaitForSingleObject(g_hMutex, INFINITE);	//获取令牌（信号）：申请信号或者线程的拥有者可以使用

	for (int i = 0; i < 10; i++)
	{
		Sleep(1000);
		printf("a进程的x线程 ： %d \n", i);
	}
	ReleaseMutex(g_hMutex);
	return 0;
}
