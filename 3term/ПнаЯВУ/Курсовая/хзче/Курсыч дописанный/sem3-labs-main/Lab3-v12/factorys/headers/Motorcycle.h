#ifndef LAB3_V12_MOTORCYCLE_H
#define LAB3_V12_MOTORCYCLE_H

#include <iostream>
#include "Moped.h"
#include "ElectricScooter.h"

using namespace std;

class Motorcycle : virtual public ElectricScooter, virtual public Moped{
protected:
    double consumption;
public:
    Motorcycle(string factoryName = "", int e = 1, string brand = "MIMSK", double maxSpeed = 110, double weight = 104.5, double power = 12, double consumption = 2.5);

    ~Motorcycle();

    void setConsumption(double value);

    double getConsumption() const;

    friend ostream &operator<<(ostream &out, Motorcycle &object);
};

#endif //LAB3_V12_MOTORCYCLE_H