#include "WristSmartWatch.h"

WristSmartWatch::WristSmartWatch() : ElectronicWatch(), hasBluetooth(false) {}

WristSmartWatch::WristSmartWatch(int h, int m, int s, bool gps, bool bluetooth)
    : ElectronicWatch(h, m, s, gps), hasBluetooth(bluetooth) {}

WristSmartWatch::~WristSmartWatch() {}

bool WristSmartWatch::getBluetoothStatus() const {
    return hasBluetooth;
}

void WristSmartWatch::setBluetoothStatus(bool status) {
    hasBluetooth = status;
}

void WristSmartWatch::display() const {
    ElectronicWatch::display();
    std::cout << "This is a Wrist Smart Watch." << std::endl;
    std::cout << "Bluetooth Status: " << (hasBluetooth ? "Enabled" : "Disabled") << std::endl;
}
