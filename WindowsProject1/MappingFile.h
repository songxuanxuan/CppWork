#pragma once
#include <Windows.h>
#include <stdio.h>

//ӳ��Ӳ���ļ����ݵ��ڴ��У�ֱ��ʹ���ڴ��ַ�����ļ�������io
class MappingFile
{
	HANDLE hFile;
	HANDLE hMapFile;
	LPVOID lpAddr;


	MappingFile(LPSTR lpcFile)
	{
		hFile = CreateFile(
			(LPWSTR)lpcFile,
			GENERIC_WRITE | GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL

		);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("createfile ʧ�� ��%d \n", GetLastError());
		}

		//����file mapping���󣬴�������ҳ���ļ�ӳ��
		hMapFile = CreateFileMapping(
			hFile,
			NULL,
			PAGE_READWRITE,
			0,
			0,
			NULL

		);
		if (hMapFile == NULL)
		{
			printf("CreateFileMapping ʧ�� ��%d \n", GetLastError());
			CloseHandle(hFile);
		}

		//����ҳӳ�䵽�����ڴ�
		lpAddr =  MapViewOfFile(
			hMapFile,
			FILE_MAP_COPY,
			0, 0, 0
		);
		if (lpAddr == NULL)
		{
			printf("CreateFileMapping ʧ�� ��%d \n", GetLastError());
			CloseHandle(hFile);
			CloseHandle(hMapFile);
		}
		//��ȡ�ļ�
		DWORD dwTest1 = *(PDWORD)lpAddr;
		DWORD dwTest2 = *((PDWORD)lpAddr + 0X1b8fc);
	}
};

