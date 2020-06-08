#include <Windows.h>
#include <stdio.h>

#define IDC_EDIT 0x100
#define IDC_BUTTON 0x101
HINSTANCE g_hInstance;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		CreateWindow(
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE,
			10,
			10,
			200,
			200,
			hwnd,
			(HMENU)IDC_EDIT,
			g_hInstance,
			NULL
		);
		CreateWindow(
			L"BUTTON",
			L"��ȡ",
			WS_CHILD | WS_VISIBLE,
			250,
			200,
			50,
			30,
			hwnd,
			(HMENU)IDC_BUTTON,
			g_hInstance,
			NULL
		);
		break;
	case WM_COMMAND: //�����Ӵ��ڸ�����Ϣ273
		/*WCHAR szBuffer1[0x80];
		wsprintf(szBuffer1, L"������  %d \n", message);
		OutputDebugString(szBuffer1);*/
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			SetDlgItemText(hwnd, IDC_EDIT, L"���Բ���");
			break;
		//case IDC_EDIT:
		//	SetDlgItemText(hwnd, IDC_EDIT, TEXT("���Բ���"));
		//	break;
		default:
			break;
		};
		break;

	case WM_KEYDOWN:
		//MessageBox(0,0,0,0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		WCHAR szBuffer[0x80];
		wsprintf(szBuffer, L"������  %c \n", wParam);
		OutputDebugString(szBuffer);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;
	CHAR szOutBuffer[0x80];
	//���崰��
	TCHAR className[] = TEXT("my first GUI");
	WNDCLASS wndClass = { 0 };
	wndClass.lpszClassName = className;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	RegisterClass(&wndClass);

	//��������ʵ����
	HWND hwnd = ::CreateWindow(
		className,
		TEXT("�ҵĴ���"),
		WS_OVERLAPPEDWINDOW,
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

	//������Ϣ������

	MSG msg;
	BOOL bRet;
	while (bRet = GetMessage(&msg, NULL, 0, 0))
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
	return 0;
}