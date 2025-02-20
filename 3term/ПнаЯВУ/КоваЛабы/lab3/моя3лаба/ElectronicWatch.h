#pragma once
#include "Watch.h"

class ElectronicWatch : public Watch {
public:
    ElectronicWatch();
    ElectronicWatch(int h, int m, int s);
    ~ElectronicWatch();

    void display() const override;
};
