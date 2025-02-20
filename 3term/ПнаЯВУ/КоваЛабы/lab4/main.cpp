// main.cpp
#include <iostream>
#include "list.h"
#include "menu.h"
using namespace std;

int main()
{
    int choice = 1;
    do
    {
        List<int>* intList = new List<int>;
        displayMenu(intList);
        delete intList;

        List<float>* floatList = new List<float>;
        displayMenu(floatList);
        delete floatList;

        cout << "Enter 1 to continue" << endl;
        cin >> choice;
    } while (choice == 1);
    return 0;
}