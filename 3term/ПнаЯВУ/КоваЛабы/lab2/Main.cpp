#include "MyString.h"

int main() {
    setlocale(LC_ALL, "Russian");

    MyString SubString;
    MyString s1, s2, s3, s4, s5, sc;
    int a, b;
    const char* strconst = "ConstStr"; 

    cout << "������� ������ s1: ";
    cin >> s1;

    cout << "������� ������ s2: ";
    cin >> s2;

    if (s1 > s2) {
        cout << "s1 > s2" << endl;
    }
    else if ((s1 < s2)) {
        cout << "s1 < s2" << endl;
    }
    else {
        cout << "s1 = s2" << endl;
    }

    sc = s1 + "Constr";
    cout << "��������� s1 + \"string\": " << sc << endl;

    s3 = s1 + s2; 
    cout << "s1 + s2: s3 = " << s3 << endl;

    s1 += s2; 
    cout << "s1 += s2: s1 = " << s1 << endl;

    s4 = ++s1;
    cout << "++s1: s4 = " << s4 << endl;

    s5 = --s1;
    cout << "--s1: s5 = " << s5 << endl;

    cout << "������� �������� �������� (1 80): ";
    cin >> a >> b;
    SubString = s1(a, b);
    cout << "���������: " << SubString << endl;

    return 0;
}