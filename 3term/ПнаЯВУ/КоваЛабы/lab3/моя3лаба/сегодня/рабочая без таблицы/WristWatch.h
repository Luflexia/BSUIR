// WristWatch.h
#pragma once
#include "MechanicalWatch.h"

class WristWatch : public MechanicalWatch {
public:
    WristWatch(int id, string brand, double price, string mechanism, string strapType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    string strapType;
};