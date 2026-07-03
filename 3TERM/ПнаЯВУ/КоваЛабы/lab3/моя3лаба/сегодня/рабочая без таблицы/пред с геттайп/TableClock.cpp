// TableClock.cpp
#include "TableClock.h"

TableClock::TableClock(int id, std::string brand, double price, std::string mechanism, bool hasAlarm)
    : MechanicalWatch(id, brand, price, mechanism), hasAlarm(hasAlarm) {}

void TableClock::display() const {
    //std::cout << left << setw(S + 8) << "TableClock";
    MechanicalWatch::display();
    std::cout << right << setw(50) << getAdditionalInfo() << left << std::endl;
}

void TableClock::update() {
    MechanicalWatch::update();
    std::cout << "It has an alarm (1 for Yes, 0 for No): ";
    std::cin >> hasAlarm;
}

std::string TableClock::getType() const {
    return "Table Clock";
}

std::string TableClock::getAdditionalInfo() const {
    return (hasAlarm ? "Has Alarm: Yes" : "Has Alarm: No");
}
