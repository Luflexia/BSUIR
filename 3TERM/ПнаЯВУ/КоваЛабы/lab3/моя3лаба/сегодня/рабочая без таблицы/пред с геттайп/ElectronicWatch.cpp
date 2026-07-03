// ElectronicWatch.cpp
#include "ElectronicWatch.h"

ElectronicWatch::ElectronicWatch(int id, std::string brand, double price, std::string displayType)
    : Watch(id, brand, price), displayType(displayType) {}

void ElectronicWatch::display() const {
    //
    Watch::display();
    std::cout << std::setw(S - 4) << "Display Type: " << displayType << left << std::endl;
}

void ElectronicWatch::update() {
    Watch::update();
    std::cout << "Enter display type: ";
    std::cin >> displayType;
}

std::string ElectronicWatch::getType() const {
    return "Electronic Watch";
}

std::string ElectronicWatch::getAdditionalInfo() const {
    return "Display Type: " + displayType;
}