#pragma once
#include "Watch.h"

class MechanicalWatch : public Watch {
public:
    MechanicalWatch();
    MechanicalWatch(int h, int m, int s);
    ~MechanicalWatch();

    void display() const override;
};
