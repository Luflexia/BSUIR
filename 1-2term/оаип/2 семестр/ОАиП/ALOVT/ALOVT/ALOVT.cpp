#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>

//------------------------------  long  ---------------------------------------------

//int main()     
//{
//	struct pole
//	{
//		unsigned i1 : 1, i2 : 1, i3 : 1, i4 : 1, i5 : 1, i6 : 1, i7 : 1, i8 : 1,
//          i9 : 1, i10 : 1, i11 : 1, i12 : 1, i13 : 1, i14 : 1, i15 : 1, i16 : 1,
//	     i17 : 1, i18 : 1, i19 : 1, i20 : 1, i21 : 1, i22 : 1, i23 : 1, i24 : 1,
//	     i25 : 1, i26 : 1, i27 : 1, i28 : 1, i29 : 1, i30 : 1, i31 : 1, i32 : 1;
//	} *p;
//
//	setlocale(0, "Rus");
//	long  k;
//	puts("������� ����� ���� LONG ");
//	scanf("%ld", &k);
//	p = (pole*)&k;
//	printf("����                        �����");
//	printf("\n %d   %d %d %d %d %d %d %d",
//		p->i32, p->i31, p->i30, p->i29, p->i28, p->i27, p->i26, p->i25);
//	printf("   %d %d %d %d %d %d %d %d",
//		p->i24, p->i23, p->i22, p->i21, p->i20, p->i19, p->i18, p->i17);
//	printf("   %d %d %d %d %d %d %d %d",
//		p->i16, p->i15, p->i14, p->i13, p->i12, p->i11, p->i10, p->i9);
//	printf("   %d %d %d %d %d %d %d %d",
//		p->i8, p->i7, p->i6, p->i5, p->i4, p->i3, p->i2, p->i1);
//	fflush(stdin);
//	getchar();
//	return 0;
//}


//------------------------------  float  -------------------------------------------

int main()     //
{
	union un
	{
		struct pole
		{
			unsigned i1 : 1, i2 : 1, i3 : 1, i4 : 1, i5 : 1, i6 : 1, i7 : 1, i8 : 1,
		i9 : 1, i10 : 1, i11 : 1, i12 : 1, i13 : 1, i14 : 1, i15 : 1, i16 : 1,
		 i17 : 1, i18 : 1, i19 : 1, i20 : 1, i21 : 1, i22 : 1, i23 : 1, i24 : 1,
		   i25 : 1, i26 : 1, i27 : 1, i28 : 1, i29 : 1, i30 : 1, i31 : 1, i32 : 1;
		} p1;     //
		struct pole2
		{
			unsigned i1 : 4, i2 : 4, i3 : 4, i4 : 4, i5 : 4, i6 : 4, i7 : 4, i8 : 4;
		} p2;
	} *u;
	setlocale(0, "Rus");

	float k;
	puts("������� ����� ���� FLOAT ");
	scanf("%f", &k);
	u = (un*)&k;
	int *pi = (int*)&k;
	system("CLS");
	printf("\n�������� �������� ���������� ����� float    %+8.4f\n\n", k);
	printf("�������� ����������� ��������� ����� float � 16 �/�    %X\n\n", *pi);

	printf("������������� ����� � ��������� �����\n");
	printf("����  ��������������                 ��������");
	printf("\n %d   %d %d %d %d %d %d %d %d",
		u->p1.i32, u->p1.i31, u->p1.i30, u->p1.i29, u->p1.i28, u->p1.i27, u->p1.i26, u->p1.i25, u->p1.i24);
	printf("   %d %d %d %d %d %d %d",
		u->p1.i23, u->p1.i22, u->p1.i21, u->p1.i20, u->p1.i19, u->p1.i18, u->p1.i17);
	printf(" %d %d %d %d %d %d %d %d",
		u->p1.i16, u->p1.i15, u->p1.i14, u->p1.i13, u->p1.i12, u->p1.i11, u->p1.i10, u->p1.i9);
	printf(" %d %d %d %d %d %d %d %d\n\n",
		u->p1.i8, u->p1.i7, u->p1.i6, u->p1.i5, u->p1.i4, u->p1.i3, u->p1.i2, u->p1.i1);

	printf("����������� ������������� �����\n");
	printf(" %d%d%d%d  %d%d%d%d  %d%d%d%d  %d%d%d%d  %d%d%d%d  %d%d%d%d  %d%d%d%d  %d%d%d%d",
		u->p1.i32, u->p1.i31, u->p1.i30, u->p1.i29, u->p1.i28, u->p1.i27, u->p1.i26, u->p1.i25, u->p1.i24,
		u->p1.i23, u->p1.i22, u->p1.i21, u->p1.i20, u->p1.i19, u->p1.i18, u->p1.i17,
		u->p1.i16, u->p1.i15, u->p1.i14, u->p1.i13, u->p1.i12, u->p1.i11, u->p1.i10, u->p1.i9,
		u->p1.i8, u->p1.i7, u->p1.i6, u->p1.i5, u->p1.i4, u->p1.i3, u->p1.i2, u->p1.i1);
	printf("\n%4X  %4X  %4X  %4X  %4X  %4X  %4X  %4X",
		u->p2.i8, u->p2.i7, u->p2.i6, u->p2.i5, u->p2.i4, u->p2.i3, u->p2.i2, u->p2.i1);
	
	fflush(stdin);
	getchar();
	return 0;
}

//------------------------------  double  -------------------------------------------

//int main()     //
//
//{
//	struct pole
//	{
//		unsigned i1 : 1, i2 : 1, i3 : 1, i4 : 1, i5 : 1, i6 : 1, i7 : 1, i8 : 1,
//	             i9 : 1, i10 : 1, i11 : 1, i12 : 1, i13 : 1, i14 : 1, i15 : 1, i16 : 1,
//	             i17 : 1, i18 : 1, i19 : 1, i20 : 1, i21 : 1, i22 : 1, i23 : 1, i24 : 1,
//	             i25 : 1, i26 : 1, i27 : 1, i28 : 1, i29 : 1, i30 : 1, i31 : 1, i32 : 1,
//	         	 i33 : 1, i34 : 1, i35 : 1, i36 : 1, i37 : 1, i38 : 1, i39 : 1, i40 : 1,
//		         i41 : 1, i42 : 1, i43 : 1, i44 : 1, i45 : 1, i46 : 1, i47 : 1, i48 : 1,
//			     i49 : 1, i50 : 1, i51 : 1, i52 : 1, i53 : 1, i54 : 1, i55 : 1, i56 : 1,
//			     i57 : 1, i58 : 1, i59 : 1, i60 : 1, i61 : 1, i62 : 1, i63 : 1, i64 : 1;
//	} *p;     //
//	setlocale(0, "Rus");
//	double k;
//	puts("������� ����� ���� DOUBLE ");
//	scanf("%lf", &k);
//	p = (pole*)&k;
//	printf("����      ��������������                 ��������");
//	printf("\n %d  %d %d %d %d %d %d %d %d %d %d %d",
//		p->i64, p->i63, p->i62, p->i61, p->i60, p->i59, p->i58,
//		p->i57, p->i56, p->i55, p->i54, p->i53);
//
//	printf("   %d %d %d %d", p->i52, p->i51, p->i50, p->i49);
//	printf(" %d %d %d %d %d %d %d %d",
//		p->i48, p->i47, p->i46, p->i45, p->i44, p->i43, p->i42, p->i41);
//	printf(" %d %d %d %d %d %d %d %d",
//		p->i40, p->i39, p->i38, p->i37, p->i36, p->i35, p->i34, p->i33);
//	printf(" %d %d %d %d %d %d", p->i32, p->i31, p->i30, p->i29, p->i28, p->i27);
//	printf("\n%27c %d %d", ' ', p->i26, p->i25);
//	printf(" %d %d %d %d %d %d %d %d",
//		p->i24, p->i23, p->i22, p->i21, p->i20, p->i19, p->i18, p->i17);
//	printf(" %d %d %d %d %d %d %d %d",
//		p->i16, p->i15, p->i14, p->i13, p->i12, p->i11, p->i10, p->i9);
//	printf(" %d %d %d %d %d %d %d %d",
//		p->i8, p->i7, p->i6, p->i5, p->i4, p->i3, p->i2, p->i1);
//	fflush(stdin);
//	getchar();
//	return 0;
//}

