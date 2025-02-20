#include "WristWatch.h"

WristWatch::WristWatch() : MechanicalWatch(), isWaterproof(false) {}

WristWatch::WristWatch(int h, int m, int s, bool waterproof) : MechanicalWatch(h, m, s), isWaterproof(waterproof) {}

WristWatch::~WristWatch() {}

bool WristWatch::getWaterproofStatus() const {
    return isWaterproof;
}

void WristWatch::setWaterproofStatus(bool status) {
    isWaterproof = status;
}

void WristWatch::display() const {
    MechanicalWatch::display();
    std::cout << "This is a Wrist Watch." << std::endl;
    std::cout << "Waterproof Status: " << (isWaterproof ? "Yes" : "No") << std::endl;
}
