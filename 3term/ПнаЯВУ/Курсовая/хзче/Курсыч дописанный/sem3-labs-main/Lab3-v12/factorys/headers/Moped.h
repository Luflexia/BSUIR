#ifndef LAB3_V12_MOPED_H
#define LAB3_V12_MOPED_H

#include <iostream>
#include "Bicycle.h"

using namespace std;

class Moped : virtual public Bicycle {
protected:
    double maxSpeed;
public:
    Moped(string factoryName = "", int e = 1, string brand = "Vespa", double maxSpeed = 5);

    ~Moped();

    void setMaxSpeed(double value);

    double getMaxSpeed() const;

    friend ostream &operator<<(ostream &out, Moped &object);
};

#endif //LAB3_V12_MOPED_H