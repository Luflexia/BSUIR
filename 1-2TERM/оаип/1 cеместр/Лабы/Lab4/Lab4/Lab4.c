#include <stdio.h>
#include <time.h>
#include <locale.h>
#define m 5

int ms[m][m];
int x = 0;
int i = 0;
int j = 0;
int k = 0;
int y = 0;
int o = 0;

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    printf("Матрица:\n");

    for (int i = 0; i < m; i++) {                 //цикл для инициализация начальной матрицы

        for (int j = 0; j < m; j++) {
            ms[i][j] = rand() % 20;
            printf("%3d ", ms[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (k = 0; k < m; k++)
    {
        for (i = 0; i < m - 1; i++)                  //Сортировка пузырьком
        {
            for (j = 0; j < m - i - 1; )
            {
                
                    if (ms[k][j] > ms[k][j + 1])
                    {
                        x = ms[k][j];
                        ms[k][j] = ms[k][j + 1];
                        ms[k][j + 1] = x;
                    }


            }
        }
    }




    for (i = 0; i < m; i++)
    {

        for (j = 0; j < m; j++)
        {
            printf("%3d ", ms[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    return 0;

}