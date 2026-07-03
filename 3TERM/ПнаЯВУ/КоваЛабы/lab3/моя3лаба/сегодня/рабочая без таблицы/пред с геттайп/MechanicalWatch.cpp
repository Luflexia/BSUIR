// MechanicalWatch.cpp
#include "MechanicalWatch.h"

MechanicalWatch::MechanicalWatch(int id, std::string brand, double price, std::string mechanism)
    : Watch(id, brand, price), mechanism(mechanism) {}

void MechanicalWatch::display() const {
    //std::cout << left <<setw(S+8)<<"MechanicalWatch";
    Watch::display();
    std::cout << std::setw(S-4) << "Mechanism: " << mechanism << std::endl;
}

void MechanicalWatch::update() {
    Watch::update();
    std::cout << "Enter mechanism: ";
    std::cin >> mechanism;
}

std::string MechanicalWatch::getType() const {
    return "Mechanical Watch";
}

std::string MechanicalWatch::getAdditionalInfo() const {
    return "Mechanism: " + mechanism;
}