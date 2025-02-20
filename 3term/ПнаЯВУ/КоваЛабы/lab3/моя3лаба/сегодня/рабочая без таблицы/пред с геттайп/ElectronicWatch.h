// ElectronicWatch.h
#pragma once
#include "Watch.h"

class ElectronicWatch : public Watch {
public:
    ElectronicWatch(int id, std::string brand, double price, std::string displayType);
    void display() const override;
    void update() override;

    std::string getType() const override;
    std::string getAdditionalInfo() const override;

private:
    std::string displayType;
};