#include <Windows.h>
#include <stdio.h>

int mainjkl()
{
	HANDLE g_hMutex = CreateMutex(NULL, true, (LPCWSTR)"xyz");		//true�����߳���ӵ���ߣ��Ѹû�����ռ��;;;�������ʧ�ܣ������Ѿ����ڵĻ�����
	//WaitForSingleObject(g_hMutex, INFINITE);	//��ȡ���ƣ��źţ��������źŻ����̵߳�ӵ���߿���ʹ��

	for (int i = 0; i < 10; i++)
	{
		Sleep(1000);
		printf("a���̵�x�߳� �� %d \n", i);
	}
	ReleaseMutex(g_hMutex);
	return 0;
}
