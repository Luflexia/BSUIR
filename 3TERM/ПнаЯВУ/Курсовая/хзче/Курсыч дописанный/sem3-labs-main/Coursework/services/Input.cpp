//
// Created by Egor on 12/6/2022.
//

#include <windows.h>
#include <iomanip>
#include "Input.h"

template<typename T>
string toString(T value) {
    std::stringstream stream;
    stream << fixed << setprecision(2) << value;
    return stream.str();
}

template<typename T>
T Input::input(T minValue, T maxValue) {
    T value;

    cin.clear();
    fflush(stdin);
    cin >> value;

    if (cin.fail())
        throw Exception("Введено неверное значение.");

    if (value > maxValue || value < minValue)
        throw Exception("Значение должно быть в пределах от " + toString(minValue) +
                        " до " + toString(maxValue) + ".", 0);
    return value;
}

int Input::inputInt(int minValue, int maxValue, const string &msg) {
    int value;

    while (true) {
        cout << msg;
        cout << pointer;

        try {
            value = input<int>(minValue, maxValue);
            break;
        } catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

float Input::inputFloat(float minValue, float maxValue, const string &msg) {
    float value;

    while (true) {
        cout << msg;
        cout << pointer;
        try {
            value = input<float>(minValue, maxValue);
            break;
        } catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

double Input::inputDouble(double minValue, double maxValue, const string &msg) {
    double value;

    while (true) {
        cout << msg;
        cout << pointer;
        try {
            value = input<double>(minValue, maxValue);
            break;
        } catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

string Input::inputMobile() {
    const string msg = "Введите ваш номер:\n+375";
    unsigned long maxValue = 999999999;
    unsigned long value;

    while (true) {
        cout << msg;
        try {
            value = input<unsigned long>(100000000, maxValue);
            break;
        } catch (Exception ex) {
            ex.what();
        }
    }
    return ("+375" + to_string(value));
}

string Input::inputEmail() {
    string email = "";

    cout << "Введите ваш email (Не обязательно):";
    cout << pointer;
    fflush(stdin);
    try {
        char c;
        while (true) {
            c = (char) getchar();
            if (c == '\n')
                break;
            if (c == EOF)
                break;
            email.push_back(c);
        }
    } catch (Exception ex) {
        ex.what();
    }

    bool isEmail = false;

    for (char c: email) {
        if (c == '@') {
            isEmail = true;
            break;
        }
    }

    if (!isEmail)
        email = "";

    return email;
}

string Input::inputString(const string &question) {
    string value;
    cout << question;
    cout << pointer;
    cin.clear();
    fflush(stdin);
    getline(cin, value);
    value = cp1251_to_utf8(value.data());
    return value;
}


bool Input::inputBool(const string &question) {
    while (true) {
        string answer = inputString(question);
        if (answer == "Да" || answer == "да" || answer == "Lf" || answer == "lf" || answer == "y" ||
            answer == "Y" || answer == "Yes" || answer == "yes")
            return true;
        if (answer == "Нет" || answer == "нет" || answer == "Ytn" || answer == "ytn" || answer == "n" ||
            answer == "N" || answer == "No" || answer == "no")
            return false;
        cout << "Непонятный ответ. \nДля ответа используйте такие слова как: да, нет." << endl;
    }
}

string Input::cp1251_to_utf8(const char *str) {
    std::string res;
    WCHAR *ures = NULL;
    char *cres = NULL;

    int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (result_u != 0) {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
            int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
            if (result_c != 0) {
                cres = new char[result_c];
                if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0)) {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;

    return res;
}