// WristSmartWatch.h
#pragma once
#include "SmartWatch.h"

class WristSmartWatch : public SmartWatch {
public:
    WristSmartWatch(int id, string brand, double price, string displayType, string features, string strapType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    string strapType;
};