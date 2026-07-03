#include <iostream>

class String {
private:
    char* str;
    int length;

public:
    // Конструкторы и деструктор
    String() : str(nullptr), length(0) {}
    String(const char* s) {
        if (s) {
            length = 0;
            while (s[length] != '\0') {
                ++length;
            }
            str = new char[length + 1];
            for (int i = 0; i < length; ++i) {
                str[i] = s[i];
            }
            str[length] = '\0';
        }
        else {
            str = nullptr;
            length = 0;
        }
    }
    String(const String& other) {
        length = other.length;
        str = new char[length + 1];
        for (int i = 0; i < length; ++i) {
            str[i] = other.str[i];
        }
        str[length] = '\0';
    }
    ~String() {
        delete[] str;
    }

    // Перегрузка операторов
    String operator+(const String& other) const {
        String result;
        result.length = length + other.length;
        result.str = new char[result.length + 1];
        for (int i = 0; i < length; ++i) {
            result.str[i] = str[i];
        }
        for (int i = 0; i < other.length; ++i) {
            result.str[length + i] = other.str[i];
        }
        result.str[result.length] = '\0';
        return result;
    }

    String& operator+=(const String& other) {
        int newLength = length + other.length;
        char* newStr = new char[newLength + 1];
        for (int i = 0; i < length; ++i) {
            newStr[i] = str[i];
        }
        for (int i = 0; i < other.length; ++i) {
            newStr[length + i] = other.str[i];
        }
        newStr[newLength] = '\0';

        delete[] str;
        str = newStr;
        length = newLength;

        return *this;
    }

    String operator++() {  // Префиксный инкремент
        for (int i = 0; i < length; ++i) {
            ++str[i];
        }
        return *this;
    }

    String operator--() {  // Префиксный декремент
        for (int i = 0; i < length; ++i) {
            --str[i];
        }
        return *this;
    }

    bool operator>(const String& other) const {
        for (int i = 0; i < length && i < other.length; ++i) {
            if (str[i] > other.str[i]) {
                return true;
            }
            else if (str[i] < other.str[i]) {
                return false;
            }
        }
        return length > other.length;
    }

    bool operator<(const String& other) const {
        for (int i = 0; i < length && i < other.length; ++i) {
            if (str[i] < other.str[i]) {
                return true;
            }
            else if (str[i] > other.str[i]) {
                return false;
            }
        }
        return length < other.length;
    }

    String operator()(int start, int end) const {
        if (start < 0 || start >= length || end < start) return String();

        int subLength = end - start + 1;
        char* subStr = new char[subLength + 1];
        for (int i = 0; i < subLength; ++i) {
            subStr[i] = str[start + i];
        }
        subStr[subLength] = '\0';

        String result(subStr);
        delete[] subStr;

        return result;
    }

    // Перегрузка оператора вывода для удобства вывода строк
    friend std::ostream& operator<<(std::ostream& out, const String& s) {
        if (s.str) {
            out << s.str;
        }
        else {
            out << "Empty String";
        }
        return out;
    }

    // Перегрузка оператора ввода
    friend std::istream& operator>>(std::istream& in, String& s) {
        char buffer[1024];  // Временный буфер для ввода
        in >> buffer;
        s = String(buffer);
        return in;
    }
};

int main() {
    String s1, s2;

    std::cout << "Введите первую строку: ";
    std::cin >> s1;

    std::cout << "Введите вторую строку: ";
    std::cin >> s2;

    String s3 = s1 + s2;
    std::cout << "s3 = " << s3 << std::endl;

    s1 += s2;
    std::cout << "s1 += s2: s1 = " << s1 << std::endl;

    String s4 = ++s1;
    std::cout << "s4 = ++s1: s4 = " << s4 << ", s1 = " << s1 << std::endl;

    String s5 = --s1;
    std::cout << "s5 = --s1: s5 = " << s5 << ", s1 = " << s1 << std::endl;

    if (s1 > s2) {
        std::cout << "s1 > s2" << std::endl;
    }
    else {
        std::cout << "s1 <= s2" << std::endl;
    }

    int start, end;
    std::cout << "Введите начальный и конечный индекс для выделения подстроки: ";
    std::cin >> start >> end;
    String s6 = s3(start, end);
    std::cout << "s6 = s3(" << start << ", " << end << "): s6 = " << s6 << std::endl;

    return 0;
}