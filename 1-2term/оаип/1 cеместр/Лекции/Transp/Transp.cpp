// ��������������� ������� ������������ ������� ���������
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#define nn 3

void main()
{
	setlocale(LC_ALL, "Russian");     // ����������� ������ ���������� �� �����
	int mas[nn][nn], i, j, kk;
	printf("\n������� �������");      // ���� �������
	for (i = 0; i<nn; i++)            // ���� �� ������� �������
	{
		printf("\n������� ������ %d  ", i);
		for (j = 0; j<nn; j++)        // ���� �� �������� �������
			scanf("%d", &mas[i][j]);
	}
	system("CLS");
	printf("\n�������� �������");     // ����� ��������� �������
	for (i = 0; i<nn; i++)            // ���� �� ������� �������
	{
		printf("\n");
		for (j = 0; j<nn; j++)        // ���� �� �������� �������
			printf("%5d", mas[i][j]);
	}
	/*************
	//       ====    for(){...}
	for(i=0;i<nn;i++)
	for(j=i+1;j<nn;j++)
	{ kk=mas[i][j];
	  mas[i][j]=mas[j][i];
	  mas[j][i]=kk;
	}
	**************
	//       ====    do{...}while()
	i=0;
	do
	{ j=i+1;
	  do
	  {  kk=mas[i][j];
	     mas[i][j]=mas[j][i];
   	     mas[j][i]=kk;
	     j++;
	  }while(j<nn);
	  i++;
	}while(i<nn);
	**************/
	//       ====    while(){...}
	i = 0;
	while (i<nn)
	{
		j = i + 1;
		while (j<nn)
		{
			kk = mas[i][j];
			mas[i][j] = mas[j][i];
			mas[j][i] = kk;
			j++;
		};
		i++;
	};

	printf("\n����������������� �������"); /*����� �������*/
	for (i = 0; i<nn; i++)          /*���� �� ������� �������*/
	{
		printf("\n");
		for (j = 0; j<nn; j++)        /*���� �� �������� �������*/
			printf("%5d", mas[i][j]);
	}
}