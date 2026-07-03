#include "SmartWatch.h"

SmartWatch::SmartWatch() : ElectronicWatch(), hasGPS(false) {}

SmartWatch::SmartWatch(int h, int m, int s, bool gps) : ElectronicWatch(h, m, s), hasGPS(gps) {}

SmartWatch::~SmartWatch() {}

bool SmartWatch::getGPSStatus() const {
    return hasGPS;
}

void SmartWatch::setGPSStatus(bool status) {
    hasGPS = status;
}

void SmartWatch::display() const {
    ElectronicWatch::display();
    std::cout << "This is a Smart Watch." << std::endl;
    std::cout << "GPS Status: " << (hasGPS ? "Enabled" : "Disabled") << std::endl;
}
