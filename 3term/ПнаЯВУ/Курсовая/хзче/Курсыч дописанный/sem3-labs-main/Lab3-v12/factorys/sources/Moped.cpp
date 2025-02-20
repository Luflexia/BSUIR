#include "../headers/Moped.h"

//  Constructors/Destructors
Moped::Moped(string factoryName, int e, string brand, double maxSpeed) {
    this->factoryName = factoryName;
    this->employees = e;
    this->brand = brand;
    this->maxSpeed = maxSpeed;
}

Moped::~Moped() {
    factoryName.clear();
    brand.clear();
};

//  Getters/Setters
void Moped::setMaxSpeed(double value) {
    maxSpeed = value;
}

double Moped::getMaxSpeed() const { return maxSpeed; }

//  Overload Method
ostream &operator<<(ostream &out, Moped &object) {
    cout << "Characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl << "\tBrand: " << object.getBrand()
         << endl << "\tMax Speed: " << object.getMaxSpeed() << " km/h"
         << endl;

    return out;
}