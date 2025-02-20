// SmartWatch.h
#pragma once
#include "ElectronicWatch.h"

class SmartWatch : public ElectronicWatch {
public:
    SmartWatch(int id, std::string brand, double price, std::string displayType, std::string features);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    std::string features;
};