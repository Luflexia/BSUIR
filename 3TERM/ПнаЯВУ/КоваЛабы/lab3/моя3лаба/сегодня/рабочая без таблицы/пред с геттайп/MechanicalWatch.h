// MechanicalWatch.h
#pragma once
#include "Watch.h"

class MechanicalWatch : public Watch {
public:
    MechanicalWatch(int id, std::string brand, double price, std::string mechanism);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    std::string mechanism;
};