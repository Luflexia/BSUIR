#ifndef LAB3_V12_SCOOTER_H
#define LAB3_V12_SCOOTER_H

#include <iostream>
#include "../../Factory.h"

using namespace std;

class Scooter : virtual public Factory {
protected:
    double weight;
public:
    Scooter(string factoryName = "", int e = 1, double weight = 13.7);

    ~Scooter();

    void setWeight(double weight);

    double getWeight() const;

    friend ostream &operator<<(ostream &out, Scooter &object);
};


#endif //LAB3_V12_SCOOTER_H
