// WristWatch.cpp
#include "WristWatch.h"

WristWatch::WristWatch(int id, std::string brand, double price, std::string mechanism, std::string strapType)
    : MechanicalWatch(id, brand, price, mechanism), strapType(strapType) {}

void WristWatch::display() const {
    //std::cout << left << setw(S + 8) << "WristWatch";
    MechanicalWatch::display();
    std::cout << right << setw(48) << "Strap Type: " << strapType << left << std::endl;
}

void WristWatch::update() {
    MechanicalWatch::update();
    std::cout << "Enter strap type: ";
    std::cin >> strapType;
}

std::string WristWatch::getType() const {
    return "Wrist Watch";
}

std::string WristWatch::getAdditionalInfo() const {
    return "Strap Type: " + strapType;
}