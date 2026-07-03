#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
// функция вычисления  SIN(X) с точностью E или количество итераций вычислений N
int main()
{
	setlocale(LC_ALL, "Russian");       // русификация вывода информации на экр
	float x, e, k = 0, st, znak = 1, y1 = 0, y2;
	int n, i = 1;
	double fakt;
	puts("\nВведите  x  e  n : ");
	scanf("%f%f%d", &x, &e, &n);
    system("CLS");                     // очистка экрана
	st= x;                             // степень Х
	do                  // цикл вычисления значения функции до заданной 
	{                   // точности (e) или до заданного числа итераций (n)
		y2 = y1;                       // запоминаем значение функции Y1
		fakt = 1;
		for (int j = 1; j <= 50; j++) // вычисление факториала (знаменатель)
		{
			fakt *= j;
			printf("\n%35.0lf", fakt);
		}
		y1 += znak*(st / fakt);        // новое значение функции Y1
		i++;                           // число выполненных итераций 
		st *= x*x;                     // вычисление степень Х (числителя)
		znak *= -1;                    // меняем знак для очередной итерации
	} while (i<n && fabs(y1 - y2)>e);  

	printf("\n f(%5.2f) = %10.7f   %d\n", x, y1, i);
		printf("\n  %ld\n", fakt);
	return 0;
}