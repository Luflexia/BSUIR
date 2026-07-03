#pragma once
#include "ElectronicWatch.h"

class SmartWatch : public ElectronicWatch {
private:
    bool hasGPS;

public:
    SmartWatch();
    SmartWatch(int h, int m, int s, bool gps);
    ~SmartWatch();

    bool getGPSStatus() const;
    void setGPSStatus(bool status);

    void display() const override;
};
