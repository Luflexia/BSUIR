// WristWatch.h
#pragma once
#include "MechanicalWatch.h"

class WristWatch : public MechanicalWatch {
public:
    WristWatch(std::string type, int id, string brand, double price, string mechanism, string strapType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        MechanicalWatch::save(out);
        out << " " << strapType;
    }

    void load(istream& in) override {
        MechanicalWatch::load(in);
        in >> strapType;
    }

    string getWatchType() const override {
        return type;
    }
private:
    string strapType;
    string type = "WristWatch";
};