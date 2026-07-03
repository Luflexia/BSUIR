// TableClock.h
#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch {
public:
    TableClock(int id, string brand, double price, string mechanism, bool hasAlarm);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    bool hasAlarm;
};