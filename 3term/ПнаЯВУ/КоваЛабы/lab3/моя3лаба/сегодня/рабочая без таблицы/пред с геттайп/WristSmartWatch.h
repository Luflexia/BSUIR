// WristSmartWatch.h
#pragma once
#include "SmartWatch.h"

class WristSmartWatch : public SmartWatch {
public:
    WristSmartWatch(int id, std::string brand, double price, std::string displayType, std::string features, std::string strapType);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    std::string strapType;
};