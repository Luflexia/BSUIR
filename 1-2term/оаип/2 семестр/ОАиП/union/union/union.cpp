#include <stdio.h>
#include <locale.h>

//int main()       
//{
//	setlocale(LC_ALL, "Russian");
//	union u_tip {
//		int i;
//		float f;
//		char s;
//	} un;
//	un.i = 1;
//	un.f = 2.1;
//	un.s = 'a';
//
//	printf("\n������ ��������� ����������� : %d  �����", sizeof(un));
//	printf("\n%d", un.i);
//	printf("\n%f", un.f);
//	printf("\n%c\n\n", un.s);
//	return 0;
//}

//----------------------------------------------------------

#define k 2
double f_var(int, ...);

struct fg {
	char name;
	double s;
	union {
		struct {
			int b;       // ��������� ��������������
			int h;       // ������
		} pr;
		struct {
			int b1, b2;  // ��������� ��������
			int h;       // ������
		} tr;
		struct {
			int r;       // ������ ����������
		} ok;
	}f;
};

int main()         //
{
	setlocale(LC_ALL, "Russian");
	struct fg un[k];
	int i;
	for (i = 0; i<k; i++)
	{
		puts("�������� ������ ��� �����    p   t   o");
		fflush(stdin);
		scanf("%c", &un[i].name);
		switch (un[i].name)
		{
		case 'p': puts("������� ��������� � ������ ���-��");
			scanf("%d%d", &un[i].f.pr.b, &un[i].f.pr.h);
			un[i].s = f_var(2, un[i].name, un[i].f.pr.b, un[i].f.pr.h);
			break;
		case 't': puts("������� 2 ��������� � ������ ��������");
			scanf("%d%d%d", &un[i].f.tr.b1, &un[i].f.tr.b2, &un[i].f.tr.h);
			un[i].s = f_var(3, un[i].name, un[i].f.tr.b1, un[i].f.tr.b2, un[i].f.tr.h);
			break;
		case 'o': puts("������� ������ ����������");
			scanf("%d", &un[i].f.ok.r);
			un[i].s = f_var(1, un[i].name, un[i].f.ok.r);
			break;
		}
	}
	printf("\n������ ����������� : %d  ", sizeof(un[i]));
	for (i = 0; i<k; i++)
	{
		printf("\n �������");
		switch (un[i].name)
		{
		case 'p': printf(" ��������������  = %5.2lf", un[i].s); break;
		case 't': printf(" ��������   = %5.2lf", un[i].s); break;
		case 'o': printf(" ����������  = %5.2lf", un[i].s); break;
		}
	}
	return 0;
}
double f_var(int kk, ...)
{
	void *p;
	int i;
	double s=0;
	char c;
	p = (void*)&kk;
	p=(int *)p+1;
	c = *((char *)p);
	p = (int *)p + 1;
	switch (c)
	{
      case 'p': for (i = 0, s=1; i < kk; i++)
	            { 
		  		  s = s**((int *)p);
				  p = (int *)p + 1;
	            }
		        break;
	  case 't': for (i = 0; i < kk-1; i++)
	            {
		          s = s+*((int *)p);
		          p = (int *)p + 1;
	            }
		        s = s / 2.0 * *((int *)p);
		        break;
	  case 'o': // for(i=0;i<kk;i++)
		        s = 3.14*(*((int *)p)* *((int *)p));
		        break;
	}
	return s;
}



