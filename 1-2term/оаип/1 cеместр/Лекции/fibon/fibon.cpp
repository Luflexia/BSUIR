#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
// ������� ���������� (������������) k ��������� 
// ������������������  ���������  1 1 2 3 5 8 13 ...
int main()               
{
	setlocale(LC_ALL, "Russian");       // ����������� ������ ���������� �� ���
    int n1, n2 = 1, n3 = 1, i, k,kk;
	do                                 // ���� ����� ����� ������ ������������������ 
	{	                               // � ��������� �� ������������ �����
		system("CLS");                 // ������� ������
		printf("������� ����� k = ");
		fflush(stdin);   // rewind(stdin); // ������ ������ �����
		n1 = scanf_s("%d", &k);        // ���� ����������� ������������������
	} while (!n1);                     // ���� ����������� ���� n1==0
	n1 = 0; kk = 0;
	for (i = 0; i<k; i++)              // ���� ������������ ������������������
	{
		n1 = i<2 ? 1 : n2 + n3;        // ���������� ���������� �������� ������������������
		printf("%15d", n1);            // ����� ���������� ��������
		kk++;                          // �������� ��������� �����
		if (kk == 7)                  // �� ����� �������� 10 ��������� ����-���
		{
			kk = 0;
			printf("\n");
		}
		n2 = n3;         // ���������� ������ n3     ( n2  n3 )                      
		n3 = n1;         // ���������� �����  n3
	}
	printf("\n");
	return 0;
}