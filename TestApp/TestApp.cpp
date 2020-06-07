#include <stdio.h>
#include <Windows.h>

int g_nNum;
int main()
{
	int i = 198;
	g_nNum = 1003;

	while (true)
	{
		printf("i = %d , addr = %X ;     g_nNum = %d , addr = %X  \n", ++i, &i, --g_nNum, &g_nNum);
		getchar();
	}
	return 0;
}