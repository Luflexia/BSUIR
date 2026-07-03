#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>

void srt1(int, int *);
void srt2(int, int *);
void srt3(int, int *);
void shaker(int, int *);
void shell(int,int *);

int main()
{
	const int n = 7;                  // размерность массива ms
	int ms[n], i, c;
	setlocale(LC_ALL, "Russian");     // русификация вывода информации на экран                                  
	do
	{
		system("CLS");
		for (i = 0; i<n; i++)
		{
			printf("ms[%2d]= ", i);
			scanf("%d", &ms[i]);
		}
		system("CLS");
		printf("\nИсходный масссив : ");  // вывод исходного массива 1
		for (i = 0; i < n; i++)          
			printf("%4d", ms[i]);
		printf("\nвыберите метод сортировки");
		printf("\n1 - через отбор");
		printf("\n2 - сортировка вставками");
		printf("\n3 - методом пузырька");
		printf("\n4 - шейкер сортировка");
		printf("\n5 - сортировка Шелла\n");
		scanf("%d", &i);
		switch (i)
		{
	       case 1: srt1(n, ms); break;
		   case 2: srt2(n, ms); break;
		   case 3: srt3(n, ms); break;
		   case 4: shaker(n, ms); break;
		   case 5: shell(n, ms); break;
		   default: printf("ошибка в выборе метода сортировки");
		}
		printf("\nИсходный масссив : ");  // вывод отсортированного массива 1
		for (i = 0; i<n; i++) printf("%3d ", ms[i]);
		fflush(stdin);
		printf("\n\nвыполнить сортировку еще (y/n).. ");
		c = getch();
	} while (c == 'y' || c == 'Y');
	return 0;
}

// сортировка массива  методом "ЧЕРЕЗ ОТБОР"
void srt1(int kl, int *ms)
{
	int i, j, m, kk;
	for (i = 0; i<kl - 1; ++i)
	{
		m = i;                     // индекс элемента для упорядочивания
		kk = ms[i];                // значение элемента
		for (j = i + 1; j<kl; ++j) // перебор последующих эл-тов справа от i-го
		if (ms[j]<kk)              // найден меньший
		{
			m = j;                 // запоминаем его индекс и значение
			kk = ms[j];
		}
		ms[m] = ms[i];             // замена выбранного и меньшего
		ms[i] = kk;
	}
}

// сортировка массива  методом " ВСТАВКИ "  
void srt2(int kl, int ms[])
{
	setlocale(LC_ALL, "RUS");	
	int i, j, kk;
	for (i = 1; i<kl; ++i)        // индекс элемента для упорядочивания
	{
		j = i - 1;                // индекс предыдущего элемента
		kk = ms[i];               // значение предыдущего элемента
		while (j >= 0 && kk<ms[j])
			ms[j-- + 1] = ms[j];  // сдвиг всех элементов направо
		ms[j + 1] = kk;           // запись в освободившийся или в тот же элемент
	}
}

// сортировка чисел методом "ПУЗЫРЬКА"
void srt3(int kl, int *ms)
{
	int i, j, m;
	for (i = 0; i<kl - 1; ++i)     // цикл выбора элементов до последнего к упорядочиванию
	for (j = kl - 1; j>i; --j)     // цикл перебора эл-тов с конечного до i-го
	{
		if (ms[j - 1]>ms[j])       // если (j-1)-й > j-го элемента, то 
		{                          // выполныется их перестановка
			m = ms[j - 1];
			ms[j - 1] = ms[j];
			ms[j] = m;
		}
	}
}

//  "Шейкер"  сортировка чисел
void shaker(int kl, int *ms)
{
	register int i, a, b, c, d;
	c = 1;
	b = kl - 1;   //номер элемента на котором остановка
	d = kl - 1;   //номер стартового элемента для сортировки справа налево
	do
	{
		for (i = d; i >= c; --i)    // цикл попарного сравнения элементов 
		{                           // справа налево по массиву
			if (ms[i - 1]>ms[i])    // выполняется условие перестановки 
			{                       // (i-1)-ого и i-го элемента 
				a = ms[i - 1];
				ms[i - 1] = ms[i];
				ms[i] = a;
				b = i;              // запоминаем крайний слева упорядоченный элемент
			}
		}
		c = b + 1;                  // номер стартового элемента для сортировки слева направо
		for (i = c; i <= d; ++i)    // аналогично предыдущему циклу, но движение
		{                           // слева направо по массиву
			if (ms[i - 1]>ms[i])
			{
				a = ms[i - 1];
				ms[i - 1] = ms[i];
				ms[i] = a;
				b = i;              // крайний слева упорядоченный элемент
			}
		}
		d = b - 1;
	} while (c <= d);
}

// "Шелл" сортировка чисел
void shell(int kl, int ms[])
{
	register int i, j, dp, a;
	for (dp = kl / 2; dp>0; dp /= 2)
	for (i = dp; i<kl; i++)
	for (j = i - dp; j >= 0 && ms[j]>ms[j + dp]; j -= dp)
	{
		a = ms[j];
		ms[j] = ms[j + dp];
		ms[j + dp] = a;
	}
}