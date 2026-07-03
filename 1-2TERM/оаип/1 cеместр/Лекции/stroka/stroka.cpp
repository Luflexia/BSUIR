//  примеры некоторых стандартных функций для работы со строками
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

int str_len(char *);
char *getstr(char *);
int get_str(char *, int);
int atoi(char *s);
void itoa(int , char *);
double atof(char *);
int str_cmp(char *, char *);
char * strcat(char *, char *, int);

int main()
{
	setlocale(LC_ALL, "Russian");
	char *s1, *s2;
	int n, n1, n2;
	double d;
	do
	{
		system("CLS");
		fflush(stdin);
		printf("введите размерность ПЕРВОЙ строки = ");
	} while (!scanf("%d", &n1) || n1 <= 0);
	if (!(s1 = (char *)calloc(n1, sizeof(char))))
	{
		printf("\nНедостаточно свободной памяти \n");
		return 0;
	}
	do
	{
		system("CLS"); 
		fflush(stdin);
		printf("введите размерность ВТОРОЙ строки = ");
	} while (!scanf("%d", &n2) || n2 <= 0);
	if (!(s2 = (char *)calloc(n2, sizeof(char))))
	{
		printf("\nНедостаточно свободной памяти \n");
		free(s1);
		return 0;
	}

	printf("Вводите первую строку  ");
	fflush(stdin);
	printf("\n строка %s", getstr(s1)); //ввод-вывод первой строки
	printf("\nДлина первой строки == %d байт", str_len(s1));
	d = atof(s1);
	printf("\nЗначение числа (double) в строке %s == %lf \n", s1, d);
	printf("Вводите вторую строку  ");
	printf("\nДлина второй строки == %d байт", get_str(s2, n2));
	printf("\nЗначение числа (int) в строке %s == %d ", s2, atoi(s2));
	printf("\nВводите число для перевода в строку  ");
	scanf("%d", &n);
	itoa(n, s2);
	printf("\n строка %s", s2);
	if (str_cmp(s1, s2)>0) printf("\nСтрока 1 > строки 2");
	else if (str_cmp(s1, s2)<0) printf("\nСтрока 1 < строки 2");
	else printf("\nСтрока 1 = строке 2");
	s1 = strcat(s1, s2, 3);                // РАЗОБРАТЬСЯ (здесь возникает проблема !!!!!!!!!!!!!)
	printf("\nстрока (s1+s2) == %s\n", s1);
	return 0;
}

// ввод строки по адресу переданному в нее
char *getstr(char *st)
{
	if (!st) return nullptr;   // передан указатель на невыделенную под строку память
	int i = 0;
	// в цикле символы заносятся в строку из буфера клавиатуры 
	// до тех пор пока не будет нажата клавиша ENTER 
	while ((*(st + i++) = (char)getchar()) != '\n');
	st[--i] = '\0';    // добавляем в конец строки символ '\0'
	return st;            // выход из функции
}

// ввод строки и подсчет ее длины
// get_str возвращает значение типа int
// в указатель st копируется указатель на строку для ввода
int get_str(char *st, int k)
{
	int c, i = 0;
//	printf("\nВводите строку  ");
	// в цикле символы заносятся в строку из буфера клавиатуры 
	// до тех пор пока не будет нажата клавиша ENTER или
	// из буфера не будет получен k-1 символ
	while (--k>0 && (c = getchar()) != EOF && c != '\n')
		st[i++] = c;    // ввод строки до заданного кол-ва символов или пока
                    	// не нажата клавиша Enter или Ctrl + Z
	st[i] = '\0';
	return i;    // возврат длины строки
}

// определение длинны строки
int str_len(char *st)
{
	int n = 0;
	for (; st[n] != '\0'; n++);
	return n;
}

// перевод цифровой символьной строки в signed int
int atoi(char *st)
{
	int n, i = 0, znak;
	while (st[i] == ' ') i++;
	if (!st[i]) return 0;              // в строке одни пробелы
	znak = (st[i] == '-') ? -1 : 1;    // запоминаем знак числа
	if (st[i] == '+' || st[i] == '-')  // пропуск знака в строке
		i++;
	for (n = 0; st[i] >= '0' && st[i] <= '9'; i++)  // выбор символа цифры числа из строки
		n = 10 * n + (st[i] - '0');    // формирование числа из цифр строки
	return znak*n;                     // возврат числа со знаком
}

// перевод числа типа signed int в символьную строку
void itoa(int n, char *st)
{
	int i = 0, znak;
	if ((znak = n)<0)            // сохраняем знак
		n *= -1;                 // n - положительное число
	do                // (ВАЖНО!!!) цифры заносим в строку в обратном порядке
	{
		st[i++] = n % 10 + '0';  // выделяется последняя справа цифра числа (как символ)
	} while ((n /= 10)>0);       // удаляется эта цифра из числа путем целочисленного деления на 10

	if (znak<0) st[i++] = '-';   // добавляем в конец строки знак числа
	st[i] = '\0';
	for (n = 0; n < i/2; n++)    // цикл переворота (реверса) полученной строки
	{
		st[n] += st[i - 1 - n];
		st[i - 1 - n] = st[n] - st[i - 1 - n];
		st[n] -= st[i - 1 - n];
	}
	//char c;                      // другой вариант цикла переворота (реверса) полученной строки
	//for (int n1 = 0, n2=i-1; n1<n2; n1++,n2--)    
	//{
	//	c = st[n1];
	//	st[n1] = st[n2];
	//	st[n2] = c;
	//}
}

// перевод цифровой символьной строки в signed double
double atof(char *ss)
{
	double n, ii = 0.0;
	int i = 0, znak;
	while (ss[i] == ' ') i++;
	if (!ss[i]) return 0.;             // в строке одни пробелы
	znak = (ss[i] == '-') ? -1 : 1;
	if (ss[i] == '+' || ss[i] == '-')  // пропуск знака
		i++;
	for (n = .0; (ss[i]>'0' && ss[i] <= '9') || ss[i] == '.'; i++)
	{
		if (ss[i] != '.')
			n = 10*n + (ss[i] - '0');     // выбор очередной цифры числа из строки
		else ii = 1;                      // выбрана десятичная точка
		if (ii) ii *= 10;   // число цифр после десятичной точки
	}
	ii /= 10;               //
	return znak*n / ii;     // возврат числа со знаком
}

// функция сравнения двух строк
int str_cmp(char *st1, char *st2)
{
	while (*st1)     // истина пока не достигнут конец ('\0') строки s1
	if (*st1 - *st2) return *st1 - *st2;  // >0 если ASCII-код  *s1 > *s2
	                                      // иначе <0
	else                                  // при совпадении символов переход
	{
		st1++;                            // к анализу следующей пары символов
		st2++;
	}
	return *st1 - *st2;  // строки либо полностью совпали либо одна окончилась
}

//  -------------------------
//  for (;*s==*t; s++,t++)
//  if(!*s) return 0;
//  return *s-*t;
//  --------------------------

// функция вставки (добавления) в строку st1 строки st2
// с позиции k
char * strcat(char *st1, char *st2, int k)
{
	char *ss;
	int i, n;
	if (k>str_len(st1)) k = str_len(st1); // проверка куда вставлять строку st2
                                          // перевыделение памяти для новой строки st1
	st1 = (char*)realloc(st1, str_len(st1) + str_len(st2) + 1);
	for (i = str_len(st1); i >= k; i--)   // сдвиг в строке st1 второй части (до позиции k) 
		*(st1 + i + str_len(st2)) = *(st1 + i); // вправо
	for (i = 0; i<str_len(st2); i++)      // перезапись второй строки st2 в строку st1
		*(st1 + k + i) = *(st2 + i);      // с позиции k
	return st1;
}

