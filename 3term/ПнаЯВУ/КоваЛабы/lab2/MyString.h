#include <iostream>
#include "StringCopy.h"

using namespace std;

class MyString {
private:
    char* str;
    int length;

public:
    MyString() : str(nullptr), length(0) {} // по умолчанию

    MyString(const char* s) { // инициализация объекта MyString по строке s (s1,s2)
        if (s) {
            length = strlen(s);
            str = new char[length + 1];
            StringCopy(str, s, length);
         /*   for (int i = 0; i < length; ++i) {
                str[i] = s[i];
            }
            str[length] = '\0';*/
        }
        else {
            str = nullptr;
            length = 0;
        }
    }

    MyString(const MyString& other) { // копирования
        length = other.length;
        str = new char[length + 1];
        StringCopy(str, other.str, length);
   /*    str[length] = '\0';*/
    }

    ~MyString() {
        delete[] str;
    }

    MyString operator+(const MyString&);
    friend MyString operator+(const char*, const MyString&);
    MyString operator()(int start, int end);
    MyString& operator+=(const MyString& other);
    MyString& operator=(const MyString& other);
    MyString operator++();
    MyString operator--();
    bool operator>(const MyString& other);
    bool operator<(const MyString& other);
    friend ostream& operator << (ostream& out, const MyString& s);
    friend std::istream& operator>>(std::istream& is, MyString& a);
};