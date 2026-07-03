#include "Factory.h"

//  Constructors/Destructors
Factory::Factory(string factoryName, int e) {
    this->factoryName = factoryName;
    employees = e;
}

Factory::~Factory() { factoryName.clear(); };

//  Getters/Setters
void Factory::setEmpoyees(int emloyees) {
    this->employees = emloyees;
}

int Factory::getEmpoyees() const { return employees; }

void Factory::setFactoryName(string name) {
    this->factoryName = name;
}

string Factory::getFactoryName() const { return factoryName; }

//  Overload Method
ostream &operator<<(ostream &out, Factory &object) {
    cout << "Factory characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl;

    return out;
}