//Main.cpp
#include "WristWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "SmartWatch.h"
#define N 1

int main()
{
	WristSmartWatch WSW[N];
	SmartWatch SW[N];
	WristWatch WW[N];
	TableClock TC[N];

	for (int i = 0; i < N; i++)
	{
		cout << "Input TableClock " << i + 1 << endl;
		cin >> TC[i];
	}
	cout << setw(SIZE + 1) << "Brand" << setw(SIZE + 1) << "Model" << setw(SIZE + 1) << "Material" << setw(SIZE + 1) << "Mechanism" << setw(SIZE + 1) << "Shape" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << TC[i] << endl;
	}

	//

	for (int i = 0; i < N; i++)
	{
		cout << "Input WristWatch " << i + 1 << endl;
		cin >> WW[i];
	}
	cout << setw(SIZE + 1) << "Brand" << setw(SIZE + 1) << "Model" << setw(SIZE + 1) << "Material" << setw(SIZE + 1) 
		<< "Mechanism" << setw(SIZE + 1) << "Strap" << setw(SIZE + 1) << "ClockFace" << endl;

	for (int i = 0; i < N; i++)
	{
		cout << WW[i] << endl;
	}

	//

	for (int i = 0; i < N; i++)
	{
		cout << "Input SmartWatch" << i + 1 << endl;
		cin >> SW[i];
	}
	cout << setw(SIZE + 1) << "Brand" << setw(SIZE + 1) << "Model" << setw(SIZE + 1) << "Screen size" << setw(SIZE + 1) << "Battery" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << SW[i] << endl;
	}

	//

	for (int i = 0; i < N; i++)
	{
		cout << "Input WristSmartWatch" << i + 1 << endl;
		cin >> WSW[i];
	}
	cout << setw(SIZE + 1) << "Brand" << setw(SIZE + 1) << "Model" << setw(SIZE + 1) << "Screen size" << setw(SIZE + 1) << "OS" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << WSW[i] << endl;
	}

	return 0;
}