// WristSmartWatch.cpp
#include "WristSmartWatch.h"

WristSmartWatch::WristSmartWatch(int id, std::string brand, double price, std::string displayType, std::string features, std::string strapType)
    : SmartWatch(id, brand, price, displayType, features), strapType(strapType) {}

void WristSmartWatch::display() const {
    //std::cout << left << setw(S + 8) << "WristSmartWatch";
    SmartWatch::display();
    std::cout << right << setw(50) << "Strap Type: " << strapType << left << std::endl;
}

void WristSmartWatch::update() {
    SmartWatch::update();
    std::cout << "Enter strap type: ";
    std::cin >> strapType;
}

std::string WristSmartWatch::getType() const {
    return "Wrist Smart Watch";
}

std::string WristSmartWatch::getAdditionalInfo() const {
    return "Strap Type: " + strapType;
}