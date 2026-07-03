// MechanicalWatch.h
#pragma once
#include "Watch.h"

class MechanicalWatch : public Watch {
public:
    MechanicalWatch(int id, string brand, double price, string mechanism);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    string mechanism;
};