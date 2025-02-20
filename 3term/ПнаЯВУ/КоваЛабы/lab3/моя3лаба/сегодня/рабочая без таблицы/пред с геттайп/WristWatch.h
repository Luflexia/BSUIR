// WristWatch.h
#pragma once
#include "MechanicalWatch.h"

class WristWatch : public MechanicalWatch {
public:
    WristWatch(int id, std::string brand, double price, std::string mechanism, std::string strapType);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    std::string strapType;
};