#include "../headers/Scooter.h"

//  Constructors/Destructors
Scooter::Scooter(string factoryName, int e, double weight) {
    this->factoryName = factoryName;
    this->employees = e;
    this->weight = weight;
}

Scooter::~Scooter() {
    factoryName.clear();
};

//  Getters/Setters
void Scooter::setWeight(double weight) {
    this->weight = weight;
}

double Scooter::getWeight() const { return weight; }

//  Overload Method
ostream &operator<<(ostream &out, Scooter &object) {
    cout << "Factory characteristics:"
         << endl << "\tFactory name: " << object.getFactoryName()
         << endl << "\tEmployees: " << object.getEmpoyees()
         << endl << "Product characteristics:"
         << endl << "\tWeight: " << object.getWeight() << " kg"
         << endl;

    return out;
}