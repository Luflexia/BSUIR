// SmartWatch.cpp
#include "SmartWatch.h"

SmartWatch::SmartWatch(int id, string brand, double price, string displayType, string features)
    : ElectronicWatch(id, brand, price, displayType), features(features) {}

void SmartWatch::display() const {
    //cout << left << setw(S + 8) << "SmartWatch";
    ElectronicWatch::display();
    cout << right << setw(50) << "Features: " << features << left << endl;
}

void SmartWatch::update() {
    ElectronicWatch::update();
    cout << "Enter features: ";
    cin >> features;
}

string SmartWatch::getAdditionalInfo() const {
    return "Features: " + features;
}