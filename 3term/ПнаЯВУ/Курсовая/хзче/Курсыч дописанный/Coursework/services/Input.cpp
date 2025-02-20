//
// Created by Egor on 12/6/2022.
//

#include "Input.h"

template<typename T>
T Input::input(T minValue, T maxValue) {
    T value;

    cin.clear();
    fflush(stdin);
    cin >> value;

    if (cin.fail())
        throw Exception("Введено неверное значение.");

//        if (maxValue != NULL && minValue != NULL)
    if (value > maxValue || value < minValue)
        throw Exception("Значение должно быть в пределах от " + to_string(minValue) + " до " +
                        to_string(maxValue) + ".", 0);
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
    const string msg = "Введите ваш email (Не обязательно):\n";
    string email = "";

    cout << msg;
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