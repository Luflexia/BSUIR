// SmartWatch.cpp
#include "SmartWatch.h"

SmartWatch::SmartWatch(int id, std::string brand, double price, std::string displayType, std::string features)
    : ElectronicWatch(id, brand, price, displayType), features(features) {}

void SmartWatch::display() const {
    //std::cout << left << setw(S + 8) << "SmartWatch";
    ElectronicWatch::display();
    std::cout << right << setw(50) << "Features: " << features << left <<std::endl;
}

void SmartWatch::update() {
    ElectronicWatch::update();
    std::cout << "Enter features: ";
    std::cin >> features;
}

std::string SmartWatch::getType() const {
    return "Smart Watch";
}

std::string SmartWatch::getAdditionalInfo() const {
    return "Features: " + features;
}