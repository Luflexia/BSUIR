#include <stdio.h>
#include <time.h>
#include <locale.h>
#define m 10

int ms[m][m];
int row = 0;
int x = 0;
int i = 0;
int j = 0;

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");

	printf("Матрица:\n");

	for (int i = 0; i < m; i++) {                 //цикл для инициализация начальной матрицы

		for (int n = 0; n < m; n++) {
			ms[i][n] = rand() % 50;
			printf("%3d ", ms[i][n]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Введите номер строки для сортировки:\n");
	scanf_s("%d", &row);

	row -= 1;

	printf("\n");

	for (i = 0; i < m - 1; i++)                  //Сортировка пузырьком
	{
		for (j = 0; j < m - i - 1; j++)
		{
			if (ms[row][j] >= ms[row][j + 1])
			{
				x = ms[row][j];
				ms[row][j] = ms[row][j + 1];
				ms[row][j + 1] = x;
			}
		}
	}

	printf("Строка отсортирована: ");

	for (int i = 0; i < m; i++) {                  //Вывод результата сортировки
		printf("%d ", ms[row][i]);
	}
	printf("\n");

	
	return 0;
}