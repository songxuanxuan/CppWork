#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	CHAR szOutBuffer[0x80];
	//定义窗口
	TCHAR className[] = TEXT("my first GUI");
	WNDCLASS wndClass = { 0 };
	wndClass.lpszClassName = className;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	RegisterClass(&wndClass);

	//创建并现实窗口
	HWND hwnd = ::CreateWindow(
		className,
		TEXT("我的窗口"),
		WS_EX_OVERLAPPEDWINDOW,
		10,
		10,
		600,
		300,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		sprintf(szOutBuffer, "Error :  %d", GetLastError());
		OutputDebugString((LPCWSTR)szOutBuffer);
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);

	//接受消息并处理

	MSG msg;
	BOOL bRet;
	if (bRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (bRet == -1)
		{
			//handl error
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
}