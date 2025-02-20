// TableClock.cpp
#include "TableClock.h"

TableClock::TableClock(int id, string brand, double price, string mechanism, bool hasAlarm)
    : MechanicalWatch(id, brand, price, mechanism), hasAlarm(hasAlarm) {}

void TableClock::display() const {
    //cout << left << setw(S + 8) << "TableClock";
    MechanicalWatch::display();
    cout << right << setw(50) << getAdditionalInfo() << left << endl;
}

void TableClock::update() {
    MechanicalWatch::update();
    cout << "It has an alarm (1 for Yes, 0 for No): ";
    cin >> hasAlarm;
}

string TableClock::getAdditionalInfo() const {
    return (hasAlarm ? "Has Alarm: Yes" : "Has Alarm: No");
}