// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <stdio.h>
#define _ONE_ 0x00007FF6A6031810 
#define _TWO_ 0x00007FF6A6031860 
#define _MYMAP_ L"MYMAP" 
DWORD WINAPI ThreadFunc(LPVOID p)
{
    LPWSTR ipBuffer;
    WCHAR dwType;
    HANDLE g_hMapFile;
    g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,(LPWSTR) _MYMAP_);
    ipBuffer = (LPWSTR)MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);
    while (true)
    {
        if (dwType != NULL)
        {
            CopyMemory(&dwType, ipBuffer, 4);
        }
        if (dwType == 1)
        {
            _asm {
                mov eax, _ONE_;
                call eax;
            }
            dwType = 0;
            CopyMemory(ipBuffer, &dwType, 4);  //用0填充还原
        }
        if (dwType == 2)
        {
            _asm {
                mov eax, _TWO_;
                call eax;
            }
            dwType = 0;
            CopyMemory(ipBuffer, &dwType, 4);
        }
        if (dwType == 3)
        {
            FreeLibraryAndExitThread((HMODULE)g_hMapFile, 0);
        }
        Sleep(1000);
    }
    
    return 0;
};


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        printf("+++++");
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
        printf("------");
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, NULL);
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

