#include "MyString.h"

MyString MyString::operator+(const MyString& other) {
    MyString result;
    result.length = length + other.length;
    result.str = new char[result.length + 1];

    StringCopy(result.str, str, length);
    StringCopy(result.str + length, other.str, other.length);

    return result;
}
MyString operator+(const char* sc, const MyString& s)
{
    MyString scres(sc);
    return scres + s;
}

    MyString& MyString::operator+=(const MyString& other) {
    
            *this = *this + other;
            return *this;
        }

    MyString& MyString::operator=(const MyString& other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (this->str != nullptr)
        {
            delete[] this->str;
        }

        this->length = other.length;
        this->str = new char[this->length + 1];
        StringCopy(this->str, other.str, length);

        return *this;
    }

    MyString MyString::operator()(int start, int end) {
        if (start < 0 || start >= length || end < start) {
            return MyString();
        }
        start -= 1;
        end -= 1;
        int subLength = end - start + 1;
        char* subStr = new char[subLength + 1];

        for (int i = 0; i < subLength; ++i) {
            subStr[i] = str[start + i];
        }
        subStr[subLength] = '\0';

        MyString result(subStr);
        delete[] subStr;

        return result;
    }

    MyString MyString::operator++() {
        for (int i = 0; i < length; ++i) {
            ++str[i];
        }
        return *this;
    }

    MyString MyString::operator--() {
        for (int i = 0; i < length; ++i) {
            --str[i];
        }
        return *this;
    }

    bool MyString::operator>(const MyString& other) {
        return length > other.length;
    }

    bool MyString::operator<(const MyString& other) {
        return length < other.length;
    }

    ostream& operator<<(ostream& out, const MyString& s) { // дружественная чтобы иметь доступ к str, length
        if (s.str) {
            out << s.str;
        }
        else {
            out << "Строка пуста";
        }
        return out; // ссылка на поток вывода
    }

    std::istream& operator >> (std::istream& str, MyString& x)
    {
        const int SIZE = 40;
        char temp[SIZE];
        rewind(stdin);
        cin.getline(temp, SIZE);
        x.length = strlen(temp);
        x.str = new char[x.length + 1];
        StringCopy(x.str, temp, x.length);
        return str;
    }