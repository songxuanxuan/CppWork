#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <fstream>
#include <string>
#define _MYMAP_ L"MYMAP" 

typedef struct 
{
	DWORD dwCreateAPIAddr;
	LPCSTR lpFileName;
	DWORD dwDesireAccess;
	DWORD dwShareMode;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes;
	DWORD dwCreationDisposition;
	DWORD dwFlagsAndAttributes;
	HANDLE hTemplateFile;
} CREATEFILE_PARAM;

typedef HANDLE (WINAPI* PFN_CreateFile)(
	LPCSTR ipFileName,
	DWORD dwDesireAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);

DWORD  WINAPI CreateFileThreadProc(LPVOID lParam)
{
	CREATEFILE_PARAM* Gcreate = (CREATEFILE_PARAM *)lParam;
	PFN_CreateFile pfnCreateFile;
	pfnCreateFile = (PFN_CreateFile)Gcreate->dwCreateAPIAddr;
	pfnCreateFile(
		Gcreate->lpFileName,
		Gcreate->dwDesireAccess,
		Gcreate->dwShareMode,
		Gcreate->lpSecurityAttributes,
		Gcreate->dwCreationDisposition,
		Gcreate->dwFlagsAndAttributes,
		Gcreate->hTemplateFile
		);
	/*printf("XXXXXXXX");*/
	return 0;
}
BOOL RemoteCreateFile(DWORD dwProcessId, char* szFilePathName)
{
	BOOL bRet;
	DWORD dwThread;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwLength;
	LPVOID lpFilePathName;
	LPVOID lpRemoteThreadAddr;
	LPVOID lpFileParamAddr;
	DWORD dwThreadFunSize;
	CREATEFILE_PARAM GCreateFile;
	HMODULE hModule;

	bRet = 0;
	hProcess = 0;
	dwThreadFunSize = 0x400;

	//��ȡ���̾��  openProcess()
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);

	if (hProcess == NULL)
	{
		printf("OpenProcess failure  \n");
		return 0;
	}
	
	//��Ŀ����̷�3���ڴ�
	//1.�����洢�ļ���
	lpFilePathName = VirtualAllocEx(hProcess, NULL, strlen(szFilePathName)+1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//2.�洢�߳�ʹ�õĺ���
	lpRemoteThreadAddr = VirtualAllocEx(hProcess, NULL, dwThreadFunSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//3.�洢����
	lpFileParamAddr = VirtualAllocEx(hProcess, NULL, sizeof(CREATEFILE_PARAM), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpFilePathName == NULL || lpRemoteThreadAddr == NULL || lpFileParamAddr == NULL)
	{
		printf("VirtualAllocEx failure  \n");
		CloseHandle(hProcess);
		return 0;
	}

	//��ʼ�� GCreateFile�Ĳ���
	GCreateFile.dwDesireAccess = GENERIC_READ | GENERIC_WRITE;
	GCreateFile.dwShareMode = 0;
	GCreateFile.lpSecurityAttributes = NULL;
	GCreateFile.dwCreationDisposition = OPEN_ALWAYS;
	GCreateFile.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	GCreateFile.hTemplateFile = NULL;
	GCreateFile.lpFileName = (LPSTR)lpFilePathName;

	//��ȡCreateFileA��ַ
	hModule = LoadLibrary(L"kernel32.dll");
	GCreateFile.dwCreateAPIAddr = (DWORD)GetProcAddress(hModule, "CreateFileA");
	FreeLibrary(hModule);

	//�޸��߳���ִ�к�������ʼ��ַ ******��jmp == e9 ,ȡe9��������ƫ�Ƽ��ϣ�5��jump����ָ��ĳ���
	DWORD dwFunAddr = (DWORD)CreateFileThreadProc;
	if (*((BYTE*)dwFunAddr) == 0xE9)
	{
		dwFunAddr = dwFunAddr + 5 + *(DWORD*)(dwFunAddr + 1);
	}

	//��ʼ����
	//�����ļ���
	WriteProcessMemory(hProcess, lpFilePathName, szFilePathName, strlen(szFilePathName) + 1, 0);
	//���ƺ���
	WriteProcessMemory(hProcess, lpRemoteThreadAddr, (LPVOID)dwFunAddr, dwThreadFunSize, 0);
	//���Ʋ���
	LPVOID llp = &GCreateFile;
	WriteProcessMemory(hProcess, lpFileParamAddr, &GCreateFile, sizeof(CREATEFILE_PARAM), 0);
	DWORD error = GetLastError();
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteThreadAddr, lpFileParamAddr, 0, &dwThread);

	CloseHandle(hProcess);
	return 1;
}
int LoadDll(DWORD dwProcessId, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwLength;
	LPTHREAD_START_ROUTINE dwLoadAddr;
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
	//dwLength = ((LPBYTE)EndThreadFunc - (LPBYTE)ThreadFunc);  //��ȡ���������ܲ���ʵ�֣���

	//��Ŀ����̷����ڴ棬����д��dll  VirtualAllocEx()
	ipAllocAddr = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (ipAllocAddr == NULL)
	{
		printf("VirtualAllocEx failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	
	//����dll·����Ŀ������ڴ�   WriteProcessMemory() ipAllocAddr, szDllPathName,dwLength
	bRet = WriteProcessMemory(hProcess, ipAllocAddr, szDllPathName, dwLength, NULL);
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
	//ע��dwordֻ��16λ����Ҫ32λ�ĵ�ַ���棬�ú����ƶ���FARPROC�������ַ����
	dwLoadAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");

	if (!dwLoadAddr)
	{
		printf("GetProcAddress failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	
	//����Զ���߳� ����loadlibraryA����dll    CreateRemoteThread()  (LPTHREAD_START_ROUTINE)dwLoadAddr
	hThread = CreateRemoteThread(hProcess, NULL, 0, dwLoadAddr, ipAllocAddr, 0, NULL);


	//hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)ipAllocAddr, NULL, 0, NULL);
	//LPDWORD threadId = 0;
	//hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, threadId);


	DWORD test = GetLastError();
	if (!hThread)
	{
		printf("CreateRemoteThread failure  \n");
		CloseHandle(hProcess);
		return 0;
	}
	//��β�ر�
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

LPTSTR ipBuffer;
HANDLE g_hMapFile;

int Init()
{
	g_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x100, _MYMAP_);
	if (g_hMapFile == NULL) return 0;
	ipBuffer = (LPTSTR)MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);
	if (ipBuffer == NULL) return 0;
	return 1;
}

int main()
{
	//DWORD dwOrderList[8] = { 1,1,1,2,2,2,1,3 };
	WCHAR procName[] = L"SimpleExp.exe";
	WCHAR dwPid = FindAProcessByName(procName);
	char path[] = "D:\\test.txt";
	RemoteCreateFile(dwPid,path);

	//char path[] = "../DllExample/x64/Debug/DllExample.dll";
	//char path[] = "E:\\CppWork\\DllExample\\Debug\\DllExample.dll";
	//LoadDll(dwPid, path);
	//DWORD dwTmp = 0;

	//if (Init())
	//{
	//	LoadDll(dwPid, path);
	//	for (int i = 0; i < 8; i++)
	//	{
	//		dwTmp = dwOrderList[i];
	//		CopyMemory(ipBuffer, &dwTmp, 4);
	//		Sleep(2000);
	//	}
	//}

	//std::string s;
	//std::ifstream iFile;
	//iFile.open("../DllExample/x64/Debug/DllExample.dll");
	//iFile >> s;
	//printf("%s   \n", s);
	//iFile.close();

	
	getchar();
	return 0;
}