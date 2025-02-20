#include "MechanicalWatch.h"

MechanicalWatch::MechanicalWatch() : Watch() {}

MechanicalWatch::MechanicalWatch(int h, int m, int s) : Watch(h, m, s) {}

MechanicalWatch::~MechanicalWatch() {}

void MechanicalWatch::display() const {
    Watch::display();
    std::cout << "This is a Mechanical Watch." << std::endl;
}
