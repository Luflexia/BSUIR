// ElectronicWatch.h
#pragma once
#include "Watch.h"

class ElectronicWatch : public Watch {
public:
    ElectronicWatch(int id, string brand, double price, string displayType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

private:
    string displayType;
};