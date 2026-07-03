// Транспонировать матрицу относительно главной диагонали
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#define nn 3

void main()
{
	setlocale(LC_ALL, "Russian");     // русификация вывода информации на экран
	int mas[nn][nn], i, j, kk;
	printf("\nВведите матрицу");      // ввод массива
	for (i = 0; i<nn; i++)            // цикл по строкам массива
	{
		printf("\nВведите строку %d  ", i);
		for (j = 0; j<nn; j++)        // цикл по столбцам массива
			scanf("%d", &mas[i][j]);
	}
	system("CLS");
	printf("\nИсходная матрица");     // вывод исходного массива
	for (i = 0; i<nn; i++)            // цикл по строкам массива
	{
		printf("\n");
		for (j = 0; j<nn; j++)        // цикл по столбцам массива
			printf("%5d", mas[i][j]);
	}
	/*************
	//       ====    for(){...}
	for(i=0;i<nn;i++)
	for(j=i+1;j<nn;j++)
	{ kk=mas[i][j];
	  mas[i][j]=mas[j][i];
	  mas[j][i]=kk;
	}
	**************
	//       ====    do{...}while()
	i=0;
	do
	{ j=i+1;
	  do
	  {  kk=mas[i][j];
	     mas[i][j]=mas[j][i];
   	     mas[j][i]=kk;
	     j++;
	  }while(j<nn);
	  i++;
	}while(i<nn);
	**************/
	//       ====    while(){...}
	i = 0;
	while (i<nn)
	{
		j = i + 1;
		while (j<nn)
		{
			kk = mas[i][j];
			mas[i][j] = mas[j][i];
			mas[j][i] = kk;
			j++;
		};
		i++;
	};

	printf("\nТранспонированная матрица"); /*вывод массива*/
	for (i = 0; i<nn; i++)          /*цикл по строкам массива*/
	{
		printf("\n");
		for (j = 0; j<nn; j++)        /*цикл по столбцам массива*/
			printf("%5d", mas[i][j]);
	}
}