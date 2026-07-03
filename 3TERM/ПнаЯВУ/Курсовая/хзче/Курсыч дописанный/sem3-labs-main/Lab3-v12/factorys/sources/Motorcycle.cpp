#include "../headers/Motorcycle.h"

//  Constructors/Destructors
Motorcycle::Motorcycle(string factoryName, int e, string brand, double maxSpeed, double weight, double power, double consumption) {
    this->factoryName = factoryName;
    this->employees = e;
    this->weight = weight;
    this->power = power;
    this->brand = brand;
    this->maxSpeed = maxSpeed;
    this->consumption = consumption;
}

Motorcycle::~Motorcycle() {
    factoryName.clear();
    brand.clear();
};

//  Getters/Setters
void Motorcycle::setConsumption(double value) {
    consumption = value;
}

double Motorcycle::getConsumption() const { return consumption; }

//  Overload Method
ostream &operator<<(ostream &out, Motorcycle &object) {
    cout << "Characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl << "Product characteristics:"
         << endl << "\tBrand: " << object.getBrand()
         << endl << "\tMax Speed: " << object.getMaxSpeed() << " km/h"
         << endl << "\tPower: " << object.getPower() << " HP"
         << endl << "\tWeight: " << object.getWeight() << " kg"
         << endl << "\tConsumption: " << object.getConsumption() << " l per 100 km"
         << endl;

    return out;
}
