#include <stdio.h>
#include <time.h>
#include <locale.h>
#define m 5


int main()
{
    int ms[m][m];

    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    printf("Начальная матрица:\n");

    for (int i = 0; i < m; i++) {                 //цикл для инициализация начальной матрицы

        for (int j = 0; j < m; j++) {
            ms[i][j] = -20 + rand() % 40;
            printf("%3d ", ms[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    // ms[i][j-y] - первый маркер; ms[i][j+1] - второй маркер


    for (int i = 0; i < m; i++)                         //цикл для сортировки
    {
        for (int k = 0; k < m - 1 + m; k++)             //пузырек
        {
            int y = 0;                                  //обнуление счетчика на случай, если цикл закончится на отрицательном числе

            for (int j = 0; j < m - 1; j++)         //пузырек
            {
                if (ms[i][j - y] >= 0)              //проверка первого маркера
                {
                    if (ms[i][j + 1] >= 0)          //проверка второго маркера
                    {
                        if (ms[i][j - y] < ms[i][j + 1])       //пузырек
                        {
                            int x = ms[i][j - y];
                            ms[i][j - y] = ms[i][j + 1];
                            ms[i][j + 1] = x;

                            y = 0;
                        }
                    }
                    else
                    {
                        y++; // счетчик остановки первого маркера
                    }
                }
            }
        }
    }

    printf("Остортированная матрица:\n");

    for (int i = 0; i < m; i++)                          //цикл для выводы отсортированной матрицы
    {

        for (int j = 0; j < m; j++)
        {
            printf("%3d ", ms[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}