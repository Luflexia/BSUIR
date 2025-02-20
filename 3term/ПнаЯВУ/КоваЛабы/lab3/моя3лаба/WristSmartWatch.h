#pragma once!!@#!@
#include "ElectronicWatch.h"

class WristSmartWatch : public ElectronicWatch {
private:
    bool hasBluetooth;

public:
    WristSmartWatch();
    WristSmartWatch(int h, int m, int s, bool gps, bool bluetooth);
    ~WristSmartWatch();

    bool getBluetoothStatus() const;
    void setBluetoothStatus(bool status);

    void display() const override;
};
