// ElectronicWatch.cpp
#include "ElectronicWatch.h"

ElectronicWatch::ElectronicWatch(int id, string brand, double price, string displayType)
    : Watch(id, brand, price), displayType(displayType) {}

void ElectronicWatch::display() const {
    //
    Watch::display();
    cout << setw(S - 4) << "Display Type: " << displayType << left << endl;
}

void ElectronicWatch::update() {
    Watch::update();
    cout << "Enter display type: ";
    cin >> displayType;
}

string ElectronicWatch::getAdditionalInfo() const {
    return "Display Type: " + displayType;
}