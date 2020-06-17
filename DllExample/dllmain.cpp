// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <stdio.h>
#define _ONE_ 0x11111111 
#define _TWO_ 0x11111112 
#define _MYMAP_ L"myMap" 
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
                mov eax, _ONE_
                call eax;
            }
            CopyMemory(ipBuffer, &dwType, 4);
        }
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
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, NULL, 0, NULL);
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

