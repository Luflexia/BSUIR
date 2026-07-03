// TableClock.h
#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch {
public:
    TableClock(int id, std::string brand, double price, std::string mechanism, bool hasAlarm);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    bool hasAlarm;
};