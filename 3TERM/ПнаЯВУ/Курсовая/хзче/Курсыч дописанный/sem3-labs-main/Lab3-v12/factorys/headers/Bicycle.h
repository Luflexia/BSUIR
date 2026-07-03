#ifndef LAB3_V12_BICYCLE_H
#define LAB3_V12_BICYCLE_H

#include <iostream>
#include "../../Factory.h"

using namespace std;

class Bicycle : virtual public Factory {
protected:
    string brand;
public:
    Bicycle(string factoryName = "", int e = 1, string brand = "Kona");

    ~Bicycle();

    void setBrand(string brand);

    string getBrand() const;

    friend ostream &operator<<(ostream &out, Bicycle &object);
};

#endif //LAB3_V12_BICYCLE_H