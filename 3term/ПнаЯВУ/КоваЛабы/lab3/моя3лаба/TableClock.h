#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch {
private:
    bool isAlarmOn;

public:
    TableClock();
    TableClock(int h, int m, int s, bool alarm);
    ~TableClock();

    bool getAlarmStatus() const;
    void setAlarmStatus(bool status);

    void display() const override;
};
