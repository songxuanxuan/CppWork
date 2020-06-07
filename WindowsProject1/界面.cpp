#include <Windows.h>


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    //定义窗口类
    TCHAR className[] = TEXT("first GUI");
    WNDCLASS wndClass = { 0 };
    wndClass.lpszClassName = className;
    wndClass.lpfnWndProc
}