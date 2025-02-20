#ifndef LAB3_V12_ELECTRICSCOOTER_H
#define LAB3_V12_ELECTRICSCOOTER_H

#include <iostream>
#include "Scooter.h"

using namespace std;

class ElectricScooter : virtual public Scooter {
protected:
    double power;
public:
    ElectricScooter(string factoryName = "", int e = 1, double weight = 13.7, double power = 350);

    ~ElectricScooter();

    void setPower(double value);

    double getPower() const;

    friend ostream &operator<<(ostream &out, ElectricScooter &object);
};

#endif //LAB3_V12_ELECTRICSCOOTER_H