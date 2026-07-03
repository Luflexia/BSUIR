#include "../headers/ElectricScooter.h"

//  Constructors/Destructors
ElectricScooter::ElectricScooter(string factoryName, int e, double weight, double power) {
    this->factoryName = factoryName;
    this->employees = e;
    this->weight = weight;
    this->power = power;
}

ElectricScooter::~ElectricScooter() {
    factoryName.clear();
};

//  Getters/Setters
void ElectricScooter::setPower(double value) {
    power = value;
}

double ElectricScooter::getPower() const { return power; }

//  Overload Method
ostream &operator<<(ostream &out, ElectricScooter &object) {
    cout << "Factory characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl << "Product characteristics:"
         << endl << "\tWeight: " << object.getWeight() << " kg"
         << endl << "\tPower: " << object.getPower() << " W"
         << endl;

    return out;
}