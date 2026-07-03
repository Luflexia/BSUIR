//Main.cpp
#include "WristWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "SmartWatch.h"
#define N 1

int main() 
{
	SmartWatch mono[N];
	WristWatch note[N];
	TableClock tab[N];

	for (int i = 0; i < N; i++)
	{
		cout << "Input Monoblock " << i+1 << endl;
		cin >> mono[i];
	}
	cout << setw(SIZE+1) << "Brand" << setw(SIZE+1) << "Model" << setw(SIZE+1) << "Corpus" << setw(SIZE+1) << "Screen size" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << mono[i] << endl;
	}



	for (int i = 0; i < N; i++)
	{
		cout << "Input Notebook " << i+1 << endl;
		cin >> note[i];
	}
	cout << setw(SIZE+1) << "Brand" << setw(SIZE+1) << "Model" << setw(SIZE+1) << "Battery" << setw(SIZE+1) 
		<< "Screen SIZE" << setw(SIZE+1) << "Keyboard" << setw(SIZE+1) << "GPU" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << note[i] << endl;
	}



	for (int i = 0; i < N; i++)
	{
		cout << "Input Tablet" << i+1 << endl;
		cin >> tab[i];
	}
	cout << setw(SIZE+1) << "Brand" << setw(SIZE+1) << "Model" << setw(SIZE+1) << "Battery" << setw(SIZE+1) << "Screen size"
		<< setw(SIZE+1) << "Kamera" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << tab[i] << endl;
	}
	
	return 0;
}