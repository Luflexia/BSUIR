// WristSmartWatch.cpp
#include "WristSmartWatch.h"

WristSmartWatch::WristSmartWatch(std::string type, int id, string brand, double price, string displayType, string features, string strapType)
    : SmartWatch(type, id, brand, price, displayType, features), strapType(strapType) {}

void WristSmartWatch::display() const {
    //cout << left << setw(S + 8) << "WristSmartWatch";
    SmartWatch::display();
    cout << right << setw(50) << "Strap Type: " << strapType << left << endl;
}

void WristSmartWatch::update() {
    SmartWatch::update();
    cout << "Enter strap type: ";
    cin >> strapType;
}

string WristSmartWatch::getAdditionalInfo() const {
    return "Strap Type: " + strapType;
}