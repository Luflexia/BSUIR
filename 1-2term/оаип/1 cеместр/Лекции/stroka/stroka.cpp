//  ������� ��������� ����������� ������� ��� ������ �� ��������
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
		printf("������� ����������� ������ ������ = ");
	} while (!scanf("%d", &n1) || n1 <= 0);
	if (!(s1 = (char *)calloc(n1, sizeof(char))))
	{
		printf("\n������������ ��������� ������ \n");
		return 0;
	}
	do
	{
		system("CLS"); 
		fflush(stdin);
		printf("������� ����������� ������ ������ = ");
	} while (!scanf("%d", &n2) || n2 <= 0);
	if (!(s2 = (char *)calloc(n2, sizeof(char))))
	{
		printf("\n������������ ��������� ������ \n");
		free(s1);
		return 0;
	}

	printf("������� ������ ������  ");
	fflush(stdin);
	printf("\n ������ %s", getstr(s1)); //����-����� ������ ������
	printf("\n����� ������ ������ == %d ����", str_len(s1));
	d = atof(s1);
	printf("\n�������� ����� (double) � ������ %s == %lf \n", s1, d);
	printf("������� ������ ������  ");
	printf("\n����� ������ ������ == %d ����", get_str(s2, n2));
	printf("\n�������� ����� (int) � ������ %s == %d ", s2, atoi(s2));
	printf("\n������� ����� ��� �������� � ������  ");
	scanf("%d", &n);
	itoa(n, s2);
	printf("\n ������ %s", s2);
	if (str_cmp(s1, s2)>0) printf("\n������ 1 > ������ 2");
	else if (str_cmp(s1, s2)<0) printf("\n������ 1 < ������ 2");
	else printf("\n������ 1 = ������ 2");
	s1 = strcat(s1, s2, 3);                // ����������� (����� ��������� �������� !!!!!!!!!!!!!)
	printf("\n������ (s1+s2) == %s\n", s1);
	return 0;
}

// ���� ������ �� ������ ����������� � ���
char *getstr(char *st)
{
	if (!st) return nullptr;   // ������� ��������� �� ������������ ��� ������ ������
	int i = 0;
	// � ����� ������� ��������� � ������ �� ������ ���������� 
	// �� ��� ��� ���� �� ����� ������ ������� ENTER 
	while ((*(st + i++) = (char)getchar()) != '\n');
	st[--i] = '\0';    // ��������� � ����� ������ ������ '\0'
	return st;            // ����� �� �������
}

// ���� ������ � ������� �� �����
// get_str ���������� �������� ���� int
// � ��������� st ���������� ��������� �� ������ ��� �����
int get_str(char *st, int k)
{
	int c, i = 0;
//	printf("\n������� ������  ");
	// � ����� ������� ��������� � ������ �� ������ ���������� 
	// �� ��� ��� ���� �� ����� ������ ������� ENTER ���
	// �� ������ �� ����� ������� k-1 ������
	while (--k>0 && (c = getchar()) != EOF && c != '\n')
		st[i++] = c;    // ���� ������ �� ��������� ���-�� �������� ��� ����
                    	// �� ������ ������� Enter ��� Ctrl + Z
	st[i] = '\0';
	return i;    // ������� ����� ������
}

// ����������� ������ ������
int str_len(char *st)
{
	int n = 0;
	for (; st[n] != '\0'; n++);
	return n;
}

// ������� �������� ���������� ������ � signed int
int atoi(char *st)
{
	int n, i = 0, znak;
	while (st[i] == ' ') i++;
	if (!st[i]) return 0;              // � ������ ���� �������
	znak = (st[i] == '-') ? -1 : 1;    // ���������� ���� �����
	if (st[i] == '+' || st[i] == '-')  // ������� ����� � ������
		i++;
	for (n = 0; st[i] >= '0' && st[i] <= '9'; i++)  // ����� ������� ����� ����� �� ������
		n = 10 * n + (st[i] - '0');    // ������������ ����� �� ���� ������
	return znak*n;                     // ������� ����� �� ������
}

// ������� ����� ���� signed int � ���������� ������
void itoa(int n, char *st)
{
	int i = 0, znak;
	if ((znak = n)<0)            // ��������� ����
		n *= -1;                 // n - ������������� �����
	do                // (�����!!!) ����� ������� � ������ � �������� �������
	{
		st[i++] = n % 10 + '0';  // ���������� ��������� ������ ����� ����� (��� ������)
	} while ((n /= 10)>0);       // ��������� ��� ����� �� ����� ����� �������������� ������� �� 10

	if (znak<0) st[i++] = '-';   // ��������� � ����� ������ ���� �����
	st[i] = '\0';
	for (n = 0; n < i/2; n++)    // ���� ���������� (�������) ���������� ������
	{
		st[n] += st[i - 1 - n];
		st[i - 1 - n] = st[n] - st[i - 1 - n];
		st[n] -= st[i - 1 - n];
	}
	//char c;                      // ������ ������� ����� ���������� (�������) ���������� ������
	//for (int n1 = 0, n2=i-1; n1<n2; n1++,n2--)    
	//{
	//	c = st[n1];
	//	st[n1] = st[n2];
	//	st[n2] = c;
	//}
}

// ������� �������� ���������� ������ � signed double
double atof(char *ss)
{
	double n, ii = 0.0;
	int i = 0, znak;
	while (ss[i] == ' ') i++;
	if (!ss[i]) return 0.;             // � ������ ���� �������
	znak = (ss[i] == '-') ? -1 : 1;
	if (ss[i] == '+' || ss[i] == '-')  // ������� �����
		i++;
	for (n = .0; (ss[i]>'0' && ss[i] <= '9') || ss[i] == '.'; i++)
	{
		if (ss[i] != '.')
			n = 10*n + (ss[i] - '0');     // ����� ��������� ����� ����� �� ������
		else ii = 1;                      // ������� ���������� �����
		if (ii) ii *= 10;   // ����� ���� ����� ���������� �����
	}
	ii /= 10;               //
	return znak*n / ii;     // ������� ����� �� ������
}

// ������� ��������� ���� �����
int str_cmp(char *st1, char *st2)
{
	while (*st1)     // ������ ���� �� ��������� ����� ('\0') ������ s1
	if (*st1 - *st2) return *st1 - *st2;  // >0 ���� ASCII-���  *s1 > *s2
	                                      // ����� <0
	else                                  // ��� ���������� �������� �������
	{
		st1++;                            // � ������� ��������� ���� ��������
		st2++;
	}
	return *st1 - *st2;  // ������ ���� ��������� ������� ���� ���� ����������
}

//  -------------------------
//  for (;*s==*t; s++,t++)
//  if(!*s) return 0;
//  return *s-*t;
//  --------------------------

// ������� ������� (����������) � ������ st1 ������ st2
// � ������� k
char * strcat(char *st1, char *st2, int k)
{
	char *ss;
	int i, n;
	if (k>str_len(st1)) k = str_len(st1); // �������� ���� ��������� ������ st2
                                          // ������������� ������ ��� ����� ������ st1
	st1 = (char*)realloc(st1, str_len(st1) + str_len(st2) + 1);
	for (i = str_len(st1); i >= k; i--)   // ����� � ������ st1 ������ ����� (�� ������� k) 
		*(st1 + i + str_len(st2)) = *(st1 + i); // ������
	for (i = 0; i<str_len(st2); i++)      // ���������� ������ ������ st2 � ������ st1
		*(st1 + k + i) = *(st2 + i);      // � ������� k
	return st1;
}

