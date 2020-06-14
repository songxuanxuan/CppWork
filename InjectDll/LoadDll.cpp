#include <Windows.h>
#include <stdio.h>

int LoadDll(DWORD dwProcessId, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwLength;
	DWORD dwLoadAddr;   
	LPVOID ipAllocAddr;   //loadlibraryA得到的地址指针
	DWORD dwThreadId;
	HMODULE hModule;  //模块 例如kernel32

	bRet = 0;
	dwLoadAddr = 0;

	//获取进程句柄  openProcess()
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);
	if (hProcess == NULL)
	{
		printf("OpenProcess failure  \n");
		return 0;
	}
	//计算dll路径的长度，并加上字符串后面0的长度 strlen() + 1
	dwLength = strlen(szDllPathName) + 1;

	//在目标进程分配内存，用于写入dll  VirtualAllocEx()
	ipAllocAddr = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_READWRITE);
	if (ipAllocAddr == NULL)
	{
		printf("VirtualAllocEx failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//拷贝dll路径到目标进程内存   WriteProcessMemory()
	bRet = WriteProcessMemory(hProcess, ipAllocAddr, &szDllPathName, dwLength, NULL);
	if (!bRet)
	{
		printf("WriteProcessMemory failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//获取(当前系统)模块（kernel32.dll）地址  GetModuleHandle()
	hModule = GetModuleHandle(L"kernel32.dll");
	if (!hModule)
	{
		printf("GetModuleHandle failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//获取(当前系统)loadlibraryA地址 GetProcAddress()  只要在同一个操作系统，内核地址都一样 
	dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryW");
	if (!dwLoadAddr)
	{
		printf("GetProcAddress failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//创建远程线程 调用loadlibraryA加载dll    CreateRemoteThread()
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwLoadAddr, ipAllocAddr, 0, NULL);
	if (!hThread)
	{
		printf("CreateRemoteThread failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//收尾关闭
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