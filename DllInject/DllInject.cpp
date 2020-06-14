// DllInject.cpp : 定义应用程序的入口点。
//

#include <Windows.h>
#include <stdio.h>
#include <synchapi.h>



int  main()
{
    for (int i = 0; i < 10; i++)
    {

        printf("+++++++++++ \n");
        Sleep(1000);
    }
    getchar();
}



