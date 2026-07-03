#include <stdio.h> 
#include <malloc.h> 
#include <iostream> 
using namespace std;

int main()
{
	int n;
	printf("Vvedite razmer:");
	scanf_s("%d", &n);
	int** pt;


	pt = (int**)malloc(n * sizeof(int*));           // Выделение памяти под указатели на строки

	for (int i = 0; i < n; i++)                     // Выделение памяти под строки
	{
		pt[i] = (int*)malloc(n * sizeof(int));
	}


	// el - элементы массива



	for (int  j = 0, i = 0, h = 0, el = 0; el < n * n;)
	{

		for (; j < n - h; j++)
			pt[i][j] = ++el;

		i++;
		j--;

		for (; i < n - h; i++)
			pt[i][j] = ++el;

		i--;
		j--;

		for (; j >= h; j--)
			pt[i][j] = ++el;

		h++;
		j++;
		i--;

		for (; i >= h; i--)
			pt[i][j] = ++el;

		i++;
		j++;


	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			printf("%-5d ", pt[i][j]);
		printf("\n");
		printf("\n");
	}

}