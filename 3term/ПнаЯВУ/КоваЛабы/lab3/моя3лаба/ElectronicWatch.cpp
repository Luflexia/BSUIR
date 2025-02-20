#include "ElectronicWatch.h"

ElectronicWatch::ElectronicWatch() : Watch() {}

ElectronicWatch::ElectronicWatch(int h, int m, int s) : Watch(h, m, s) {}

ElectronicWatch::~ElectronicWatch() {}

void ElectronicWatch::display() const {
    Watch::display();
    std::cout << "This is an Electronic Watch." << std::endl;
}
