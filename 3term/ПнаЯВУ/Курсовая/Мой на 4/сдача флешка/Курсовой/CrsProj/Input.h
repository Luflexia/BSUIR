#ifndef COURSEWORK_INPUT_H
#define COURSEWORK_INPUT_H
#include <string>
#include <iomanip>
#include <iostream>
#include "Exception.h"
#include <sstream>
#include <codecvt>


using namespace std;

class Input {

    const string pointer = "\n\033[38;2;128;0;128m> \033[0m"; // ANSI escape code для цвета

    template<typename T>
    T input(T minValue = NULL, T maxValue = NULL);

public:
    int inputInt(int minValue, int maxValue, const string& msg = "");

    float inputFloat(float minValue, float maxValue, const string& msg = "");

    double inputDouble(double minValue, double maxValue, const string& msg = "");

    string inputMobile();

    string inputEmail();

    string inputString(const string& question);

    bool inputBool(const string& question);

    template<typename T>
    string toStringWithPrecision(T value, int precision);

    template<typename T>
    string toString(T value) {
        std::stringstream stream;
        stream << fixed << setprecision(2) << value;
        return stream.str();
    }

    string cp1251_to_utf8(const std::string& cp1251Str);
};

#endif //COURSEWORK_INPUT_H