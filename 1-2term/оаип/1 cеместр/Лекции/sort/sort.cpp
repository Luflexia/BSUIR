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
	const int n = 7;                  // ����������� ������� ms
	int ms[n], i, c;
	setlocale(LC_ALL, "Russian");     // ����������� ������ ���������� �� �����                                  
	do
	{
		system("CLS");
		for (i = 0; i<n; i++)
		{
			printf("ms[%2d]= ", i);
			scanf("%d", &ms[i]);
		}
		system("CLS");
		printf("\n�������� ������� : ");  // ����� ��������� ������� 1
		for (i = 0; i < n; i++)          
			printf("%4d", ms[i]);
		printf("\n�������� ����� ����������");
		printf("\n1 - ����� �����");
		printf("\n2 - ���������� ���������");
		printf("\n3 - ������� ��������");
		printf("\n4 - ������ ����������");
		printf("\n5 - ���������� �����\n");
		scanf("%d", &i);
		switch (i)
		{
	       case 1: srt1(n, ms); break;
		   case 2: srt2(n, ms); break;
		   case 3: srt3(n, ms); break;
		   case 4: shaker(n, ms); break;
		   case 5: shell(n, ms); break;
		   default: printf("������ � ������ ������ ����������");
		}
		printf("\n�������� ������� : ");  // ����� ���������������� ������� 1
		for (i = 0; i<n; i++) printf("%3d ", ms[i]);
		fflush(stdin);
		printf("\n\n��������� ���������� ��� (y/n).. ");
		c = getch();
	} while (c == 'y' || c == 'Y');
	return 0;
}

// ���������� �������  ������� "����� �����"
void srt1(int kl, int *ms)
{
	int i, j, m, kk;
	for (i = 0; i<kl - 1; ++i)
	{
		m = i;                     // ������ �������� ��� ��������������
		kk = ms[i];                // �������� ��������
		for (j = i + 1; j<kl; ++j) // ������� ����������� ��-��� ������ �� i-��
		if (ms[j]<kk)              // ������ �������
		{
			m = j;                 // ���������� ��� ������ � ��������
			kk = ms[j];
		}
		ms[m] = ms[i];             // ������ ���������� � ��������
		ms[i] = kk;
	}
}

// ���������� �������  ������� " ������� "  
void srt2(int kl, int ms[])
{
	setlocale(LC_ALL, "RUS");	
	int i, j, kk;
	for (i = 1; i<kl; ++i)        // ������ �������� ��� ��������������
	{
		j = i - 1;                // ������ ����������� ��������
		kk = ms[i];               // �������� ����������� ��������
		while (j >= 0 && kk<ms[j])
			ms[j-- + 1] = ms[j];  // ����� ���� ��������� �������
		ms[j + 1] = kk;           // ������ � �������������� ��� � ��� �� �������
	}
}

// ���������� ����� ������� "��������"
void srt3(int kl, int *ms)
{
	int i, j, m;
	for (i = 0; i<kl - 1; ++i)     // ���� ������ ��������� �� ���������� � ��������������
	for (j = kl - 1; j>i; --j)     // ���� �������� ��-��� � ��������� �� i-��
	{
		if (ms[j - 1]>ms[j])       // ���� (j-1)-� > j-�� ��������, �� 
		{                          // ����������� �� ������������
			m = ms[j - 1];
			ms[j - 1] = ms[j];
			ms[j] = m;
		}
	}
}

//  "������"  ���������� �����
void shaker(int kl, int *ms)
{
	register int i, a, b, c, d;
	c = 1;
	b = kl - 1;   //����� �������� �� ������� ���������
	d = kl - 1;   //����� ���������� �������� ��� ���������� ������ ������
	do
	{
		for (i = d; i >= c; --i)    // ���� ��������� ��������� ��������� 
		{                           // ������ ������ �� �������
			if (ms[i - 1]>ms[i])    // ����������� ������� ������������ 
			{                       // (i-1)-��� � i-�� �������� 
				a = ms[i - 1];
				ms[i - 1] = ms[i];
				ms[i] = a;
				b = i;              // ���������� ������� ����� ������������� �������
			}
		}
		c = b + 1;                  // ����� ���������� �������� ��� ���������� ����� �������
		for (i = c; i <= d; ++i)    // ���������� ����������� �����, �� ��������
		{                           // ����� ������� �� �������
			if (ms[i - 1]>ms[i])
			{
				a = ms[i - 1];
				ms[i - 1] = ms[i];
				ms[i] = a;
				b = i;              // ������� ����� ������������� �������
			}
		}
		d = b - 1;
	} while (c <= d);
}

// "����" ���������� �����
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