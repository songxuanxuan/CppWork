#pragma once
#include <Windows.h>
class MemSearch
{
	DWORD g_arList[1024];	//��ַ�б�
	int g_nListCnt = 0;		//��ַ����
	HANDLE g_hProcess;  //Ŀ�����

public:
	VOID SetG_hProcess(HANDLE g_hProcess);
	BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);
	BOOL FindFirst(DWORD dwValue) ;
	BOOL FindNext(DWORD dwValue) ;
	VOID ShowList();
};

