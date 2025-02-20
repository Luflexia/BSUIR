#include "Array.h"
#include <iostream>

using namespace std;


int main()
{
	int quit = 1;
	while (quit)
	{
		int m = 0, n = 0;
		cout << "Размер матрицы a:" << endl;
		cin >> m >> n;
		Array a(m, n);
		cout << "Заполнение матрицы a:" << m << "x" << n << endl;
		a.input();
		a.output();
		cout << "Размер матрицы b:" << endl;
		cin >> m >> n;
		Array b(m, n);
		cout << "Заполнение матрицы b:" << m << "x" << n << endl;
		b.input();
		b.output();
		Array c;
		cout << "Произведение матриц:" << endl;
		c.multiply(a, b);
		c.output();
		cout << "Продолжить - введите 1, выйти - введите 0" << endl;
		cin >> quit;
	}
	return 0;
}

