// DllInject.cpp : 定义应用程序的入口点。
//

#include <Windows.h>
#include <stdio.h>
#include <synchapi.h>
#include <iostream>

void getOne()
{
	printf("1111-----\n");
}
void getTwo()
{
	printf("2222-----\n");
}

int main()
{
	int input;
	printf("源程序———————— \n");
	while (scanf_s("%d", &input))
	{
		switch (input)
		{
		case 1:
			getOne();
			break;
		case 2:
			getTwo();
			break;

		default:
			break;
		}
		
	}

}



