#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <io.h>
// ------------------------------------------  БИНАРНЫЙ ФАЙЛ ------------------------------------------------

// вводим в БИНАРНЫЙ файл несколько чисел по возрастанию, 
// добавить в файл числа размещая их без нарушения упорядоченности

//void print(FILE *f)
//{
//	int j;
//	if (!f)
//	{
//		puts("файл не открыт");
//		return;
//	}
//	rewind(f);
//	do
//	{
//		fread(&j, sizeof(int), 1, f);
//		if (feof(f))  break;
//		printf("%3d", j);
//	} while (1);
//	printf("\n");
//}
//
//int main()
//{
//  setlocale(LC_ALL, "Russian");
//  FILE *f;
//  int ms[] = { 5, 7, 10, 13, 14 };
//  int i, j;
//  fpos_t l1, l2;     // тип позиция в файле
//  char name[20];
//  puts("введите имя файла ");
//  scanf("%s", name);
//  f = fopen(name, "w+b");
//  fwrite(ms, sizeof(int), sizeof(ms) / sizeof(int), f); //
//  print(f);
//  while (1)
//  {
//	puts("введите очередное число ");
//	scanf("%d", &i);
//	if (i == 999) break;               // выход из программы
//	rewind(f);	                       // установка УТПФ в начало файла и сброс
//	do                                 // признака конца файла
//	{
//	    fgetpos(f, &l1);               // УТПФ наначало поля считываемого числа
//	    fread(&j, sizeof(int), 1, f);
//		if (feof(f) || j>i)  break;    // достигнут конец файла или считано число
//		                               // большее чем введенное с клавиатуры.
//	} while (1);
//	rewind(f);
//	if (j<i)                           // EOF и в файле нет числа > чем введенное
//	{
//		fseek(f, 0, 2);                // выход на конец файла
//		fwrite(&i, sizeof(int), 1, f); // дозапись в конец файла
//		continue;
//	}
//	fseek(f, -sizeof(int), 2);         // УТПФ на последний элемент файла
//	do
//	{
//		fgetpos(f, &l2);               // сдвиг всех чисел в массиве до
//		fread(&j, sizeof(int), 1, f);  // l2 позиция УТПФ числа которое > чем i
//		rewind(f);
//		l2 += sizeof(int);
//		fsetpos(f, &l2);
//		fwrite(&j, sizeof(int), 1, f);
//		fseek(f, l2 - 2 * sizeof(int), 0);
//	} while (l1<l2 - sizeof(int));
//	fseek(f, l2 - sizeof(int), 0);     // запись i на место числа с которого
//	fwrite(&i, sizeof(int), 1, f);     // произведен сдвиг всех чисел вниз
//	print(f);
//  }	                                   
//  //rewind(f);
//  //do
//  //{
//	 // fread(&j, sizeof(int), 1, f);
//	 // if(feof(f))  break;
//	 // printf("%3d", j);
//  //} while (1);
//  //printf("\n");
//  // ******* print(f);
//  fclose(f);
//  return 0;
//}

//============================================================================================

// имеются 2 файла(fl1 и fl2) оба упорядочены по возрастанию,
// переписать их в результирующий без нарушения упорядоченности

//int main()
//{
//  setlocale(LC_ALL, "Russian");
//  FILE *f1, *f2, *f3;
//  int i1, i2;
//  int m1[] = { 1, 2, 4, 5 },
//	    m2[] = { 1, 3 };
//	  char cc;
//  if (!(f1 = fopen("fl1", "w+b")) ||
//	    !(f2 = fopen("fl2", "wb+")) ||
//	    !(f3 = fopen("fl3", "wb")))
//  {
//	  puts("error");
//	  return 0;
//  }
//  // запись в первый файл элементов массива m1 
//  for (i1 = 0; i1<sizeof(m1) / sizeof(int); i1++) fwrite(&m1[i1], sizeof(int), 1, f1);
//  // запись во второй файл элементов массива m2
//  fwrite(m2, sizeof(m2), 1, f2);
//
//  rewind(f1); rewind(f2);            // УТПФ в начало обоих файлов
//  fread(&i1, sizeof(int), 1, f1);    // чтение первого элемента из первого файла
//  fread(&i2, sizeof(int), 1, f2);    // чтение первого элемента из второго файла
//  do
//  {
//	  while (!feof(f1) && i1<i2)            // цикл выполняется пока первый файл не окончен
//	  {                                     // и из него прочитано меньшее число
//		  fwrite(&i1, sizeof(int), 1, f3);  // запись в третий файл из первого
//		  fread(&i1, sizeof(int), 1, f1);   // чтение очередного числа из первого файла
//	  }
//	  if (feof(f1)) break;                  // выход из do ... while если первый файл окончен
//
//	  while (!feof(f2) && i1 >= i2)         // цикл выполняется пока второй файл не окончен
//	  {                                     // и из него прочитано не большее число
//		  fwrite(&i2, sizeof(int), 1, f3);  // запись в третий файл из второго
//		  fread(&i2, sizeof(int), 1, f2);   // чтение очередного числа из второго файла
//	  }
//  } while (!feof(f2));
//
//  while (!feof(f1))                         // если первый файл не окончился
//  {                                         // то перезапись из него в третий файл
//	  fwrite(&i1, sizeof(int), 1, f3);      // запись в f3
//	  fread(&i1, sizeof(int), 1, f1);       // чтение из f1
//  }
//
//  while (!feof(f2))                         // если первый файл не окончился
//  {                                         // то перезапись из него в третий файл
//	  fwrite(&i2, sizeof(int), 1, f3);      // запись в f3
//	  fread(&i2, sizeof(int), 1, f2);       // чтение из f2
//  }
//
//  fclose(f1);
//  fclose(f2);
//  fclose(f3);
//          // вывод на экран результирующего файла
//  f3 = fopen("fl3", "rb");
//  while (1)
//  {
//	  fread(&i1, sizeof(int), 1, f3);
//	  if (feof(f3)) break;
//	  printf("%3d", i1);
//  }
//  printf("\n");
//  return 0;
//}

//============================================================================================

// имеются 2 файла (a и aa) по возрастанию и по убыванию
// переписать их в результирующий по возрастанию 

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f1, *f2, *f3;
//	int i1, i2, k, kk;
//	fpos_t n;
//	int m1[] = { 1, 2 },
//		m2[] = { 7, 5, 4, 3, 1 };
//	char cc;
//	if (!(f1 = fopen("a", "w+b")) ||
//		!(f2 = fopen("aa", "wb+")) ||
//		!(f3 = fopen("aaa", "wb")))
//	{
//		puts("error");
//		return 0;
//	}
//	for (i1 = 0; i1<sizeof(m1) / sizeof(int); i1++) fwrite(&m1[i1], sizeof(int), 1, f1);
//	fwrite(m2, sizeof(m2), 1, f2);
//
//	rewind(f1);
//	fread(&i1, sizeof(int), 1, f1);
//	k = sizeof(int);
//	fseek(f2, -k, 2);  
//	fgetpos(f2, &n);
//	fread(&i2, sizeof(int), 1, f2);
//	do
//	{
//		while (!feof(f1) && i1<i2)
//		{
//			fwrite(&i1, sizeof(int), 1, f3);
//			fread(&i1, sizeof(int), 1, f1);
//		}
//		if (feof(f1)) break;
//
//		while (n >= 0 && i1 >= i2)
//		{
//			fwrite(&i2, sizeof(int), 1, f3);
//			fseek(f2, -k * 2, 1);
//			fgetpos(f2, &n);
//			if (n<0) break;
//			fread(&i2, sizeof(int), 1, f2);
//		}
//	} while (n >= 0);
//
//	while (!feof(f1))
//	{
//		fwrite(&i1, sizeof(int), 1, f3);
//		fread(&i1, sizeof(int), 1, f1);
//	}
//
//	while (n >= 0)
//	{
//		fwrite(&i2, sizeof(int), 1, f3);
//		if (n<=0) break;
//		fseek(f2, -k*2, 1);
//		fgetpos(f2, &n);
//		fread(&i2, sizeof(int), 1, f2);
//	}
//
//	fclose(f1);
//	fclose(f2);
//	fclose(f3);
//
//	f3 = fopen("aaa", "rb");
//	while (1)
//	{
//		fread(&i1, sizeof(int), 1, f3);
//		if (feof(f3)) break;
//		printf("%3d", i1);
//	}
//	printf("\n");
//	return 0;
//}


//============================================================================================

// Пример программы, выполняющей работу с файлом содержащим структуры,
// например, некоторые параметры компьютера
// режимы работы: создание/просмотр/удаление информации из базы данных (файла)

//#include <stdlib.h>
//#include <string.h>
//
//void add(char *);
//void del(char *);
//void print(char *);
//void swap(FILE *ff, struct str, struct str, fpos_t *, fpos_t *);
//void zamena(char *);
//
//struct inf {
//	char tp[15];        // название
//	unsigned short pr;  // процессор
//};
//struct str {
//	inf in;
//	double hd;          // HDD
//	int mem;            // память
//};
//
//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	FILE *f;
//	char name[] = "baza";
//	int i, k, pl;
//	while (1)
//	{
//		puts("\n1- создать/добавить \n2- \n3-удалить \n4-вывести на экран \n5-выход\n выберите режим работы");
//		scanf("%d", &k);
//		switch (k)
//		{
//			case 1: add(name); break;
//			case 3: del(name); break;
//			case 2: zamena(name); break;
//			case 4: print(name); break;
//			case 5: return 0;
//		}
//	}
//	return 0;
//}
//
//void add(char *name)
//{
//	double a;
//	str *s;
//	FILE *f;
//	if ((f = fopen(name, "ab")) == nullptr)
//	{	puts(" ошибка  открытия файла");
//		return;
//	}
//	if ((s = (str *)calloc(1, sizeof(str))) == NULL)
//	{	puts("ошибка при выделении памяти");
//		fclose(f);
//		return;
//	}
//	do
//	{	fflush(stdin);
//		puts("введите тип компъютера ");
//		gets(s->in.tp); if (!strcmp(s->in.tp, "")) break;
//		puts("введите частоту проц. , объем  HDD   и  памяти");
//		scanf("%d", &(s->in.pr));
//		scanf("%lf", &a);        // ???????/
//		s->hd = a;
//		scanf("%d", &s->mem);
//		fwrite(s, sizeof(str), 1, f);   // дозапись структуры в конец файла
//	} while (1);
//	fclose(f);
//}
//
//void del(char *name)
//{
//	FILE *f;
//	str st, stt;
//	fpos_t l;
//	if ((f = fopen(name, "r+b")) == nullptr)
//	{
//		puts(" ошибка  открытия файла");
//		return;
//	}
//	puts("\nвведите тип компъютера : "); scanf("%s", st.in.tp);
//	puts("\nвведите частоту проц.  : "); scanf("%d%", &st.in.pr);
//	puts("\nвведите объем  HDD     : "); scanf("%lf", &st.hd);
//	puts("\nвведите объем  памяти  : "); scanf("%d",  &st.mem);
//	do
//	{	fread(&stt, sizeof(str), 1, f);
//		if (feof(f)) break;
//		if (!strcmp(st.in.tp, stt.in.tp) && st.in.pr == stt.in.pr &&
//			st.hd == stt.hd && st.mem == stt.mem)
//		{                  // найдена удаляемая структура   sst
//			do
//			{	fread(&stt, sizeof(str), 1, f);          // чтение следующей стр-ры
//				if (feof(f)) break;
//				l = ftell(f);                            // позиция за считанной
//				fseek(f, l - (fpos_t)(2 * sizeof(str)), 0);// на 2 стр-ры назад к началу файла
//				fwrite(&stt, sizeof(str), 1, f);         // и запись считанной ст-ры
//				fseek(f, l, 0);                          // возврат к позиции L
//			} while (!feof(f));
//		}
//	} while(1);
//	rewind(f);
//	fseek(f, -sizeof(str), 2);
//	l = ftell(f);
//	chsize(fileno(f),l);
//	fclose(f);
//}
//
//void print(char *name)
//{
//	FILE *f;
//	str st;
//	int k = 0;
//	if ((f = fopen(name, "rb")) == nullptr)
//	{
//		puts(" ошибка  открытия файла");
//		return;
//	}
//	puts("   тип компъютера    процессор     HDD    RAM");
//	do
//	{
//		k++;              // счетчик числа выведенных на экран записей
//		fread(&st, sizeof(str), 1, f);
//		if (feof(f)) break;
//		if (k >= 10)         // выведено 5 записей
//		{
//			k = 0;             // сброс четчика
//			getch();         // задержка вывода на экран
//			puts("   тип компъютера    процессор     HDD    RAM");
//		}
//		printf("\n%20s%10d%8.2lf%8d", st.in.tp, st.in.pr, st.hd, st.mem);
//	} while (!feof(f));
//	fclose(f);
//}
//
//void zamena(char *name)
//{
//	FILE *f;
//	str st1,st2;
//	fpos_t n1, n2;
//	int k = 0;
//	if ((f = fopen(name, "r+b")) == nullptr)
//	{
//		puts(" ошибка  открытия файла");
//		return;
//	}
//	do
//	{ 
//		fgetpos(f,&n1);
//		k=fread(&st1, sizeof(str), 1, f);
//		if (feof(f)) break;
//		fgetpos(f, &n2);
//		k=fread(&st2, sizeof(str), 1, f);
//		if (feof(f)) break;
//		swap(f, st1, st2, &n1, &n2);
//		n1 = n2 + sizeof(str);
//		fsetpos(f,&n1);
//	} while (1);
//	fclose(f);
//}
//
//void swap(FILE *ff, struct str s1, struct str s2, fpos_t *l1, fpos_t *l2)
//{
//	if (!ff) return;
//	fsetpos(ff, l1);
//	fwrite(&s2, sizeof(str), 1, ff);    // вторая стр-ра по первому адресу
//	fsetpos(ff, l2);
//	fwrite(&s1, sizeof(str), 1, ff);    // первая стр-ра по второму адресу
//}

