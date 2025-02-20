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

public:
    int inputInt(int minValue, int maxValue, const string &msg = "");

    string inputMobile();

    string inputEmail();
};

#endif //COURSEWORK_INPUT_H