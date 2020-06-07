#pragma once
#include <Windows.h>
class MemSearch
{
	DWORD g_arList[1024];	//地址列表
	int g_nListCnt = 0;		//地址数量
	HANDLE g_hProcess;  //目标进程

public:
	VOID SetG_hProcess(HANDLE g_hProcess);
	BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);
	BOOL FindFirst(DWORD dwValue) ;
	BOOL FindNext(DWORD dwValue) ;
	VOID ShowList();
};

