#include "TableClock.h"

TableClock::TableClock() : MechanicalWatch(), isAlarmOn(false) {}

TableClock::TableClock(int h, int m, int s, bool alarm) : MechanicalWatch(h, m, s), isAlarmOn(alarm) {}

TableClock::~TableClock() {}

bool TableClock::getAlarmStatus() const {
    return isAlarmOn;
}

void TableClock::setAlarmStatus(bool status) {
    isAlarmOn = status;
}

void TableClock::display() const {
    MechanicalWatch::display();
    std::cout << "This is a Table Clock." << std::endl;
    std::cout << "Alarm Status: " << (isAlarmOn ? "On" : "Off") << std::endl;
}
