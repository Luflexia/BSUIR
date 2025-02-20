#include <stdio.h>
#include <locale.h>
int main()
{
    setlocale(LC_ALL, "Russian");       // русификация вывода информации на экр
	short int k;
	double d;
	char c;
	long int i,j;
	int ind;
	printf(" вводите ..... ");
	ind = scanf("%hd%lf%ld%c%ld",&k,&d,&i,&c,&j);
//	ind = scanf_s("%hd%lf%ld%c%ld",&k,&d,&i,&c,1,&j);
	printf(" %hd   %lf   %ld  %c  %ld\n",k,d,i,c,j);
	return 0;
}
