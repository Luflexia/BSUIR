// MechanicalWatch.h
#pragma once
#include "Watch.h"

class MechanicalWatch : public Watch {
public:
    MechanicalWatch(string type, int id, string brand, double price, string mechanism);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        Watch::save(out);
        out << " " << mechanism;
    }

    void load(istream& in) override {
        Watch::load(in);
        in >> mechanism;
    }

    string getWatchType() const override {
        return type;
    }

private:
    string mechanism;
    string type = "MechanicalWatch";
};