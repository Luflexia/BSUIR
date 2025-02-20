#include "Watch.h"
#include <iostream>
#include <iomanip>

Watch::Watch() : hour(0), minute(0), second(0) {}

Watch::Watch(int h, int m, int s) : hour(h), minute(m), second(s) {}

Watch::~Watch() {}

int Watch::getHour() const {
    return hour;
}

int Watch::getMinute() const {
    return minute;
}

int Watch::getSecond() const {
    return second;
}

void Watch::setHour(int h) {
    hour = h;
}

void Watch::setMinute(int m) {
    minute = m;
}

void Watch::setSecond(int s) {
    second = s;
}

void Watch::display() const {
    // Вывод шапки таблицы
    std::cout << std::setw(10) << "Hour"
        << std::setw(10) << "Minute"
        << std::setw(10) << "Second" << std::endl;

    // Вывод данных объекта
    std::cout << std::setw(10) << hour
        << std::setw(10) << minute
        << std::setw(10) << second << std::endl;
}
