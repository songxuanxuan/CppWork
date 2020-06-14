#include <Windows.h>
#include <stdio.h>

int LoadDll(DWORD dwProcessId, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwLength;
	DWORD dwLoadAddr;   
	LPVOID ipAllocAddr;   //loadlibraryA�õ��ĵ�ַָ��
	DWORD dwThreadId;
	HMODULE hModule;  //ģ�� ����kernel32

	bRet = 0;
	dwLoadAddr = 0;

	//��ȡ���̾��  openProcess()
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);
	if (hProcess == NULL)
	{
		printf("OpenProcess failure  \n");
		return 0;
	}
	//����dll·���ĳ��ȣ��������ַ�������0�ĳ��� strlen() + 1
	dwLength = strlen(szDllPathName) + 1;

	//��Ŀ����̷����ڴ棬����д��dll  VirtualAllocEx()
	ipAllocAddr = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_READWRITE);
	if (ipAllocAddr == NULL)
	{
		printf("VirtualAllocEx failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//����dll·����Ŀ������ڴ�   WriteProcessMemory()
	bRet = WriteProcessMemory(hProcess, ipAllocAddr, &szDllPathName, dwLength, NULL);
	if (!bRet)
	{
		printf("WriteProcessMemory failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//��ȡ(��ǰϵͳ)ģ�飨kernel32.dll����ַ  GetModuleHandle()
	hModule = GetModuleHandle(L"kernel32.dll");
	if (!hModule)
	{
		printf("GetModuleHandle failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//��ȡ(��ǰϵͳ)loadlibraryA��ַ GetProcAddress()  ֻҪ��ͬһ������ϵͳ���ں˵�ַ��һ�� 
	dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryW");
	if (!dwLoadAddr)
	{
		printf("GetProcAddress failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//����Զ���߳� ����loadlibraryA����dll    CreateRemoteThread()
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwLoadAddr, ipAllocAddr, 0, NULL);
	if (!hThread)
	{
		printf("CreateRemoteThread failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//��β�ر�
	CloseHandle(hThread);
	//CloseHandle(hProcess);
	return 1;
}

int main()
{
	char path[] = "E:\\CppWork\\DllExample\\x64\\Debug\\DllExample.dll";
	LoadDll(16740, path);
	getchar();
	return 0;
}