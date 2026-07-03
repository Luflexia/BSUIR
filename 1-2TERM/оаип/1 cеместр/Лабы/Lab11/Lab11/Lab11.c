#include <stdio.h>
#include <locale.h>

int y = 1;

int vvod(int x)                                     //  <
{                                                   //  |
    if (x > 0)                                      //  |
    {                                               //  |
            int n;                                  //  |
                      //  |
            printf("Введите число %d :", y);        //  |
            y++;                                    //  |  счетчик номера числа
            scanf_s("%d", &n);                      //  |
            vvod(x - 1);                            //  |
            printf("%d", "[",x "]",n);
    }
    else
    {
        printf("Чисел нет\n");
    }
    

    
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int x;

    printf("Введите кол-во элементов:\n");
    scanf_s("%d", &x);

    vvod(x);  

    return 0;
}

