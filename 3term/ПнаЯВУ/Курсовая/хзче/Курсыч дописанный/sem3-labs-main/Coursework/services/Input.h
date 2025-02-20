//
// Created by Egor on 12/6/2022.
//

#ifndef COURSEWORK_INPUT_H
#define COURSEWORK_INPUT_H

#include <string>
#include <iostream>
#include "Exception.h"

using namespace std;

class Input {
    const string pointer = "\n> ";

    template<typename T>
    T input(T minValue = NULL, T maxValue = NULL);

    string cp1251_to_utf8(const char *str);
public:
    int inputInt(int minValue, int maxValue, const string &msg = "");

    float inputFloat(float minValue, float maxValue, const string &msg = "");

    double inputDouble(double minValue, double maxValue, const string &msg = "");

    string inputMobile();

    string inputEmail();

    string inputString(const string &question);

    bool inputBool(const string &question);
};

#endif //COURSEWORK_INPUT_H