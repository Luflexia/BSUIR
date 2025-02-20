#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

char *str1;
char *str2;
int n;
int x = 0;

int main()
{
	printf("Vvedite dlinu stroki: ");
	scanf_s("%d", &n);
	str1 = (char*)calloc(n+1, sizeof(char));
	str2 = (char*)calloc(n+1, sizeof(char));
	printf("Vvedite 1st stroku: ");
	getchar();
	gets_s(str1, n+1);
	printf("Vvedite 2st stroku: ");
	gets_s(str2, n+1);

	del();

	printf("Stroka 3: ");
	puts(str1);

	return 0;
}

int del()
{
	int i, j, x;
	for (i = 0; *(str2 + i) != '\0'; i++)                             // цикл по строке 2
	{
		for (j = 0; *(str1 + j) != '\0'; j++)                       // цикл по строке 1
		{
			if (*(str2 + i) == *(str1 + j))                       // если символ из строки 2 совпадаетс символом из строки 1
			{
				for (x = j; *(str1 + x) != '\0'; x++)           // начиная со следующего символа
					*(str1 + x) = *(str1 + x + 1);             // сместить все символы на один символ влево
				j--;
			}
		}
	}
	return 0;
}