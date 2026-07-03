// WristWatch.cpp
#include "WristWatch.h"

WristWatch::WristWatch(int id, string brand, double price, string mechanism, string strapType)
    : MechanicalWatch(id, brand, price, mechanism), strapType(strapType) {}

void WristWatch::display() const {
    //cout << left << setw(S + 8) << "WristWatch";
    MechanicalWatch::display();
    cout << right << setw(48) << "Strap Type: " << strapType << left << endl;
}

void WristWatch::update() {
    MechanicalWatch::update();
    cout << "Enter strap type: ";
    cin >> strapType;
}

string WristWatch::getAdditionalInfo() const {
    return "Strap Type: " + strapType;
}