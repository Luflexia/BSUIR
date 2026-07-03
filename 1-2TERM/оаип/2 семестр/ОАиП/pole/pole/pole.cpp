#include <stdio.h>
#include <conio.h>
#include <locale.h>

//=================================  pole 1  ========================================
struct pole
{
	int i : 1;
	unsigned int j : 3;
	int : 9;
	unsigned  int kk : 5;
	int k : 14;
} pl;

int main()
{
	setlocale(LC_ALL, "Russian");
	printf("\nsizeof(pole) = %d байт\n", sizeof(pole));
	pl.i = 3;      //  13 == 1 1 0 1        12 =  1 1 0 0 
	pl.j = 7;
	pl.k = 3;
	printf("i= %d  j= %d  k=%d\n", pl.i, pl.j, pl.k);
	printf("sizeof(pl)= %d байт\n", sizeof(pl));
	return 0;
}

//=================================  pole 2  ========================================

// использование полей бит для отображения битового значения переменных
//struct pole
//{
//	unsigned int i1 : 1, i2 : 1, i3 : 1, i4 : 1, i5 : 1, i6 : 1,
//                 i7 : 1, i8 : 1, i9 : 1, i10 : 1, i11 : 1, i12 : 1,
//                 i13 : 1, i14 : 1, i15 : 1, i16 : 1;
//};
//void print(pole *pl)
//{
//	printf("\n %d %d %d %d %d %d %d %d    %d %d %d %d %d %d %d %d\n",
//	pl->i16, pl->i15, pl->i14, pl->i13, pl->i12, pl->i11, pl->i10, pl->i9,
//	pl->i8, pl->i7, pl->i6, pl->i5, pl->i4, pl->i3, pl->i2, pl->i1);
//}
//
//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	pole *pl;
//	short h = 21;
//
//
//	pl = (struct pole *)&h;
//	printf("\n   sizeof(pole)= %d байт\n", sizeof(pole));
//	printf("\n   sizeof(pl)= %d байт\n",   sizeof(*pl));
//	printf("\n   sizeof(h)= %d байт\n",    sizeof(h));
//	print(pl);
//	print((struct pole *)&h);
//	((struct pole *)&h)->i1++;
//	print(pl);
//
//	return 0;
//}

//=================================  pole 3  ========================================

//  выравнивание на границу слова
//struct pole
//{
//	int p1 : 1;
//	int : 0;
//	unsigned int p2 : 2;
//	int : 6;
//	int p3 : 4;
//} pl = { 1, 2, 3 };
//
//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	pole *p;
//	p = (pole*)&pl;
//	printf("\nsize = %d \n", sizeof(pole));
//	printf("\n %d  %d  %d \n", pl.p1, pl.p2, pl.p3);
//	return 0;
//}


//=================================  pole 5  ========================================
//struct p_bit
//{
//  unsigned int i1  : 1, i2  : 1, i3  : 1, i4  : 1, i5  : 1, i6  : 1, 
//               i7  : 1, i8  : 1, i9  : 1, i10 : 1, i11 : 1, i12 : 1,
//               i13 : 1, i14 : 1, i15 : 1, i16 : 1;
//};
//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	p_bit *p;
//	short int a = 5;
//	p = (p_bit*)&a;
//	printf("\n a=%d", a);
//	printf("\n %d %d %d %d %d %d %d %d    %d %d %d %d %d %d %d %d\n\n",
//		p->i16, p->i15, p->i14, p->i13, p->i12, p->i11, p->i10, p->i9,
//		p->i8, p->i7, p->i6, p->i5, p->i4, p->i3, p->i2, p->i1);
//	a = ~a;                  //   инверсия всей переменной   а
//	printf("\n a=%d", a);
//	printf("\n %d %d %d %d %d %d %d %d    %d %d %d %d %d %d %d %d\n\n",
//		p->i16, p->i15, p->i14, p->i13, p->i12, p->i11, p->i10, p->i9,
//		p->i8, p->i7, p->i6, p->i5, p->i4, p->i3, p->i2, p->i1);
//	p->i1 = ~p->i1;          //   инверсия первого бита ( i1 ) переменной
//	printf("\n a=%d", a);
//	printf("\n %d %d %d %d %d %d %d %d    %d %d %d %d %d %d %d %d\n\n",
//		p->i16, p->i15, p->i14, p->i13, p->i12, p->i11, p->i10, p->i9,
//		p->i8, p->i7, p->i6, p->i5, p->i4, p->i3, p->i2, p->i1);
//	return 0;
//}

//=================================  pole 4  ========================================

// вычисление остатка от деления на 2 , 4

//#define size sizeof(int)*8
//
//struct pole
//{
//	union
//	{
//		struct
//		{
//			unsigned i1 : 1;
//			int : 1;
//		} st1;
//		struct
//		{
//			unsigned i1 : 2;
//		}st2;
//	} un;
//	int: size - 2;
//};
//
//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	pole *pl;
//	int k = 27;
//	pl = (pole *)&k;
//	printf("\nостаток от деления на 2 =%d\n", pl->un.st1.i1);
//	printf("остаток от деления на 4 =%d\n", pl->un.st2.i1);
//	return 0;
//}