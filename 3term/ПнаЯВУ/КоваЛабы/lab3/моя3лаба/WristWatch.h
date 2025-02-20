#pragma once
#include "MechanicalWatch.h"

class WristWatch : public MechanicalWatch {
private:
    bool isWaterproof;

public:
    WristWatch();
    WristWatch(int h, int m, int s, bool waterproof);
    ~WristWatch();

    bool getWaterproofStatus() const;
    void setWaterproofStatus(bool status);

    void display() const override;
};
