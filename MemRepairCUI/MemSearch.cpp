#include "MemSearch.h"
#include <stdio.h>

VOID MemSearch::SetG_hProcess(HANDLE g_hProcess)
{
	this->g_hProcess = g_hProcess;
}

BOOL MemSearch::CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	BYTE arByte[4096];	//windowsÿҳ��4k����ҳ����Ч�ʸ�
	if (!::ReadProcessMemory(this->g_hProcess, (LPCVOID)dwBaseAddr, arByte, 4096, NULL))
	{
		return FALSE;
	}
	DWORD* pdw;
	for (int i = 0; i < 4*1024-3; i++)		//i++��ÿ��byte���Ƚ��𣿣���
	{
		pdw = (DWORD*)&arByte[i];
		if (*pdw == dwValue)
		{
			if (MemSearch::g_nListCnt>=1024)
			{
				return	FALSE;
			}
			MemSearch::g_arList[MemSearch::g_nListCnt++] = dwBaseAddr + i;
		}
	}
	return TRUE;
}
BOOL MemSearch::FindFirst(DWORD dwValue) 
{
	const DWORD dwOneGB = 1024 * 1024 * 1024;
	const DWORD dwOnePage = 4 * 1024;

	if (g_hProcess == NULL)
		return FALSE;

	//�鿴����ϵͳ��������ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		dwBase = 4 * 1024 * 1024; //win98
	else
		dwBase = 64 * 1024;	//win nt 

	//�ӿ�ʼ��ַ��2gb��ʼ����

	for (; dwBase < 2*dwOneGB; dwBase+=dwOnePage)
	{
		CompareAPage(dwBase, dwValue);
	}
	return TRUE;
}
BOOL MemSearch::FindNext(DWORD dwValue)
{
	return 0;
}

VOID MemSearch::ShowList()
{
	for (int i = 0; i < MemSearch::g_nListCnt; i++)
	{
		printf("%X \n", MemSearch::g_arList[i]);
	}
	
}
