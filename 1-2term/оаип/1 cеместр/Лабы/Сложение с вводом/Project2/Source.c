#include <stdio.h>

int main()
{
	int x, y, z;
	printf("Vvedite 4islo:\n");
	scanf_s("%d", &x);
	printf("Vvedite 4islo:\n");
	scanf_s("%d", &y);
	z = x + y;
	printf("Summa=%d\n", z);
	z = x * y; 
	printf("Umnozhit=%d\n", z);


	int i = 7; while (i++ <= 10);
	printf("Umnozhit=%d\n", i);
	    return 0;
}