#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <fstream>
#include <string>

DWORD WINAPI ThreadFunc(LPVOID p) 
{
	printf("-----");
	return 0;
};

int LoadDll(DWORD dwProcessId, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwLength;
	LPTHREAD_START_ROUTINE dwLoadAddr;
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
	//dwLength = strlen(szDllPathName) + 1;
	dwLength = sizeof(szDllPathName);
	//在目标进程分配内存，用于写入dll  VirtualAllocEx()
	ipAllocAddr = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (ipAllocAddr == NULL)
	{
		printf("VirtualAllocEx failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//拷贝dll路径到目标进程内存   WriteProcessMemory()
	bRet = WriteProcessMemory(hProcess, ipAllocAddr, szDllPathName, dwLength, NULL);
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
	//注意dword只有16位，需要32位的地址来存，用函数制定的FARPROC，否则地址不对
	dwLoadAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryW");
	if (!dwLoadAddr)
	{
		printf("GetProcAddress failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//创建远程线程 调用loadlibraryA加载dll    CreateRemoteThread()  (LPTHREAD_START_ROUTINE)dwLoadAddr
	hThread = CreateRemoteThread(hProcess, NULL, 0, dwLoadAddr, ipAllocAddr, 0, NULL);
	if (!hThread)
	{
		printf("CreateRemoteThread failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//收尾关闭
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return 1;
}

DWORD FindAProcessByName(WCHAR* processName)
{
	HRESULT hr = E_FAIL;
	DWORD dwPid = 0;
	PROCESSENTRY32 pe = { 0 };
	HANDLE hSnapshot = NULL;
	
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	while (Process32Next(hSnapshot, &pe))
	{
		
		if (!wcscmp(pe.szExeFile, processName))
		{
			dwPid = pe.th32ProcessID;
			wprintf(L"name: %s   pid: %d  \n", pe.szExeFile, pe.th32ProcessID);
		}
	}
	return dwPid;
}

int main()
{
	/*WCHAR procName[] = L"SimpleExp.exe";
	WCHAR dwPid = FindAProcessByName(procName);
	char path[] = "../DllExample/x64/Debug/DllExample.dll";
	LoadDll(dwPid, path);*/
	//std::string s;
	//std::ifstream iFile;
	//iFile.open("../DllExample/x64/Debug/DllExample.dll");
	//iFile >> s;
	//printf("%s   \n", s);
	//iFile.close();

	LPDWORD threadId = 0;
	CreateThread(NULL, 0, ThreadFunc, NULL, 0, threadId);
	DWORD test = GetLastError();
	
	getchar();
	return 0;
}