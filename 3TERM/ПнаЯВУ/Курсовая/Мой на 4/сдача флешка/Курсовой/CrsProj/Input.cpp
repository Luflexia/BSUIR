#include "Input.h"
#include "Exception.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <codecvt>
#include <algorithm>

template<typename T>
std::string Input::toStringWithPrecision(T value, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
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

int Input::inputInt(int minValue, int maxValue, const string& msg) {
    int value;

    while (true) {
        cout << msg;
        cout << pointer;

        try {
            value = input<int>(minValue, maxValue);
            break;
        }
        catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

float Input::inputFloat(float minValue, float maxValue, const string& msg) {
    float value;

    while (true) {
        cout << msg;
        cout << pointer;
        try {
            value = input<float>(minValue, maxValue);
            //string svalue = toStringWithPrecision(value, 2);
            //value = std::stof(svalue);
            break;
        }
        catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

double Input::inputDouble(double minValue, double maxValue, const string& msg) {
    double value;

    while (true) {
        cout << msg;
        cout << pointer;
        try {
            value = input<double>(minValue, maxValue);
            //string svalue = toStringWithPrecision(value, 2);
            //value = stof(svalue);
            break;
        }
        catch (Exception ex) {
            ex.what();
        }
    }
    return value;
}

string Input::inputMobile() {
    const string msg = "Введите ваш номер:\n+375";
    unsigned long minValue = 100000000;
    unsigned long maxValue = 999999999;
    unsigned long value;

    while (true) {
        cout << msg;
        try {
            value = input<unsigned long>(minValue, maxValue);
            break;
        }
        catch (Exception ex) {
            ex.what();
        }
    }
    return ("+375" + to_string(value));
}

string Input::inputEmail() {
    string email = "";
    cout << "Желаете ввести email?";
    if (inputBool("")) {
        try {
            cout << "Введите ваш email:";
            //cout << pointer;
            email = inputString("");
        }
        catch (Exception ex) {
            ex.what();
        }
    }

    return email;
}

string Input::inputString(const string& question) {
    string value;
    cout << question;
    cout << pointer;
    cin.clear();
    //fflush(stdin);
    getline(cin, value);
    return value;
}


bool Input::inputBool(const string& question) {
    while (true) {
        string answer = inputString(question);
        if (answer == "Да" || answer == "да" || answer == "Lf" || answer == "lf" || answer == "y" ||
            answer == "Y" || answer == "Yes" || answer == "yes" || answer == "д" || answer == "Д") {
            return true;
        }
        else if (answer == "Нет" || answer == "нет" || answer == "Ytn" || answer == "ytn" || answer == "n" ||
            answer == "N" || answer == "No" || answer == "no" || answer == "н" || answer == "Н") {
            return false;
        }
        else {
            cout << "Для ответа используйте: да, нет." << endl;
        }
    }
}

string Input::cp1251_to_utf8(const std::string& cp1251Str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring utf16 = converter.from_bytes(cp1251Str);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf16Converter;
    std::string utf8 = utf16Converter.to_bytes(utf16);

    return utf8;
}