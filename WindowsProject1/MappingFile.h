#pragma once
#include <Windows.h>
#include <stdio.h>

//映射硬盘文件内容到内存中，直接使用内存地址访问文件，不用io
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
			printf("createfile 失败 ：%d \n", GetLastError());
		}

		//创建file mapping对象，创建物理页和文件映射
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
			printf("CreateFileMapping 失败 ：%d \n", GetLastError());
			CloseHandle(hFile);
		}

		//物理页映射到虚拟内存
		lpAddr =  MapViewOfFile(
			hMapFile,
			FILE_MAP_COPY,
			0, 0, 0
		);
		if (lpAddr == NULL)
		{
			printf("CreateFileMapping 失败 ：%d \n", GetLastError());
			CloseHandle(hFile);
			CloseHandle(hMapFile);
		}
		//读取文件
		DWORD dwTest1 = *(PDWORD)lpAddr;
		DWORD dwTest2 = *((PDWORD)lpAddr + 0X1b8fc);
	}
};

