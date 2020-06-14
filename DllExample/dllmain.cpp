// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <stdio.h>

DWORD WINAPI ThreadProc(DWORD dWord)
{
    for (int i = 0; i < 10; i++)
    {
        printf("inject ------------>DLL_THREAD_ATTACH \n");
        Sleep(1000);
    }
    return 0;
}
DWORD WINAPI ThreadProc2(DWORD dWord)
{
    for (int i = 0; i < 10; i++)
    {
        printf("inject ------------> DLL_PROCESS_ATTACH \n");
        Sleep(1000);
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, NULL);
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

