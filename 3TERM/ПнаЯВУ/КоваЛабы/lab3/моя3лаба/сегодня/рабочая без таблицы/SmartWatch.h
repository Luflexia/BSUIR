// SmartWatch.h
#pragma once
#include "ElectronicWatch.h"

class SmartWatch : public ElectronicWatch {
public:
    SmartWatch(int id, string brand, double price, string displayType, string features);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    string features;
};