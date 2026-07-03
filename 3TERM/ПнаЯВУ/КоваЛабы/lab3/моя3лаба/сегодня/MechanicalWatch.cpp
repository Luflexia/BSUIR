// MechanicalWatch.cpp
#include "MechanicalWatch.h"

MechanicalWatch::MechanicalWatch(string type, int id, string brand, double price, string mechanism)
    : Watch(type, id, brand, price), mechanism(mechanism) {}

void MechanicalWatch::display() const {
    //cout << left <<setw(S+8)<<"MechanicalWatch";
    Watch::display();
    cout << setw(S - 4) << "Mechanism: " << mechanism << endl;
}

void MechanicalWatch::update() {
    Watch::update();
    cout << "Enter mechanism: ";
    cin >> mechanism;
}

string MechanicalWatch::getAdditionalInfo() const {
    return "Mechanism: " + mechanism;
}