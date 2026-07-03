#include <stdio.h>
#include <time.h>
#include <locale.h>
#define N 10



int ms[N];
int x = 0;
int i = 0;
int j = 0;

int main() 
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
    printf("Исходный массив:\n");

    for (i = 0; i < N; i++)
    {               
        ms[i] = rand() % 20;                  //Случайная инициализация массива
        printf("%d ", ms[i]);

    }

    int el = ms[0];
    printf("\n\nПервый элемент массива равен %d", el);

    for (i = 0; i < N - 1; i++)               //Сортировка по убыванию от второго элемента
    {
        for (j = 1; j < N - i - 1; j++)
        {
            if (ms[j] < ms[j+1])
            {
                x = ms[j];
                ms[j] = ms[j+1];
                ms[j+1] = x;
            }
        }

    }

    printf("\n\nУпорядоченный массив по убыванию без первого элемента:\n");

    for (i = 0; i < N; i++) {                  //Вывод результата сортировки по убыванию от второго элемента
        printf("%d ", ms[i]);
    }

        for (j = 0; j < N - 1; j++)            //Поиск места для первого элемента массива
        {
            if (el <= ms[j + 1])
            {
                x = ms[j];
                ms[j] = ms[j + 1];
                ms[j + 1] = x;
            }
        }
   
    printf("\n\nИтоговый массив:\n");

    for (i = 0; i < N; i++) {            //Вывод результата
        printf("%d ", ms[i]);
    }
    return 0;
}