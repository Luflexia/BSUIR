#include <iostream>
#include <limits>
#include "Int.h"

#define A_VALUE 300

using namespace std;

int checkInt() {
    int Integer;
    while (true) {
        if (cin >> Integer)
            return Integer;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cout << endl << "\tError, B must be of type Integer." << endl << "Please try again: ";
    }
}

string boolToString(bool b) {
    if (b)
        return "True";
    return "False";
}

Int intToClass(int a) {
    return Int(a);
}

int menu() {
    Int a(A_VALUE), b;
    cout << "For overload use A = " << a.getValue() << " and B." << endl
         << "Enter B, for further work. B = ";
    b.setValue(checkInt());

    system("cls");

    cout << endl << "A = " << a.getValue()
         << endl << "B = " << b.getValue()
         << endl << "\tOverloading methods:"
         << endl << "Operator \"=\", \"A = B\" as a result: A = " << (a = b).getValue() << endl
         << endl << "Operator \"[]\", \"A[2]\"as a result: " << a[1] << endl
         << endl << "Operator \"()\", \"A(30)\" as a result: A = " << a(30).getValue() << endl;
    a.setValue(A_VALUE);
    cout << endl << "Operator \"++\", \"A++\" as a result: A = " << (a++).getValue()
         << endl << "                                  A = " << a.getValue()
         << endl << "               \"++A\" as a result: A = " << (++a).getValue() << endl;
    a.setValue(A_VALUE);
    cout << endl << "Operator \"+\", \"A + B\" as a result: " << (a + b).getValue() << endl

         << endl << "Operator \">\", \"A > B\" as a result: " << boolToString(a > b) << endl

         << endl << "\tOverloading friend methods:"
         << endl << "Operator \"--\", \"A--\" as a result: A = " << (a--).getValue()
         << endl << "                                  A = " << a.getValue()
         << endl << "               \"--A\" as a result: A = " << (--a).getValue() << endl;
    a.setValue(A_VALUE);
    cout << endl << "Operator \"+\", \"505 - A\" as a result: " << (505 - a).getValue() << endl
         << endl << "Operator \">\", \"A < 100\" as a result: " << boolToString(a < 100) << endl
         << endl << "Operator \"<<\", as a result: " << a << endl;
    return 0;
}

int main() {
    return menu();
}