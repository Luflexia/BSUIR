#include <stdio.h>
#include <time.h>
int main()
{
	srand(time(NULL));
	int A[4][5]; //��������� �������
	int B[5] = { 0,0,0,0,0 }; //��������� ������
	int A1[4] = { 0,0,0,0 }; //����� �������

	printf("Matrix:\n");

	//���� ��� ������������� ��������� �������
	for (int m = 0; m < 4; m++) {
	
		for (int n = 0; n < 5; n++) {
			A[m][n] = -100 + rand() % 200;
			printf("%d ", A[m][n]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Vector:\n");

	
	//���� ��� ������������� ���������� �������
	for (int o = 0; o < 5; o++) {
		B[o] = rand() % 10;
		printf("%d ", B[o]);
	};

	printf("\n\n");

	//���� ��� ��������� ������� A[4][5] �� ������ B[5]
	for (int i= 0; i < 4; i++) {
		for (int j = 0; j < 5; j++)
			A1[i] += A[i][j] * B[i];
	};

	//���������� ��������� � ����� ������� � �� �����
	printf("Result:\n ");
	for (int n=0; n<4; n++)
	printf("%d ", A1[n]);
	printf("\n\n\n ");
	system("pause");
	return 0;
}

/*
	//���� ��� ��������� ������� A[4][5] �� ������ B[5]
	for (int i = 0; i < 5; i++) {
		A1[0] += A[0][i] * B[i]; };
	for (int i = 0; i < 5; i++) {
		A1[1] += A[1][i] * B[i]; };
	for (int i = 0; i < 5; i++) {
		A1[2] += A[2][i] * B[i]; };
	for (int i = 0; i < 5; i++) {
		A1[3] += A[3][i] * B[i]; };
	*/