#include "../headers/Bicycle.h"

//  Constructors/Destructors
Bicycle::Bicycle(string factoryName, int e, string brand) {
    this->factoryName = factoryName;
    this->employees = e;
    this->brand = brand;
}

Bicycle::~Bicycle() {
    factoryName.clear();
    brand.clear();
};

//  Getters/Setters
void Bicycle::setBrand(string brand) {
    this->brand = brand;
}

string Bicycle::getBrand() const { return brand; }

//  Overload Method
ostream &operator<<(ostream &out, Bicycle &object) {
    cout << "Characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl << "\tBrand: " << object.getBrand()
         << endl;

    return out;
}