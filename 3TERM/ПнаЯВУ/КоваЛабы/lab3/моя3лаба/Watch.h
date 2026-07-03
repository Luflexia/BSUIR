#pragma once
using namespace std;
#include <iostream>

class Watch {
protected:
    int hour;
    int minute;
    int second;

public:
    Watch();
    Watch(int h, int m, int s);
    virtual ~Watch();

    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    void setHour(int h);
    void setMinute(int m);
    void setSecond(int s);

    virtual void display() const;
};
