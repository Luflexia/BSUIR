#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <locale.h>

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ifstream file("code.txt");
	ofstream final_file("final_code.txt");
	do
	{
		//string str;
		char str[256];
		//getline(file, str);
		file.getline(str, 256);
		int j = 0;
		while (str[j * 4] == ' ') j++;
		//for (int i = j * 2; i < str.length(); i++)
		for (int i = j * 2; str[i]; i++)
			final_file << str[i];
		final_file << '\n';
	} while (!file.eof());

	return 0;
}
