// SmartWatch.h
#pragma once
#include "ElectronicWatch.h"

class SmartWatch : public ElectronicWatch {
public:
    SmartWatch(std::string type, int id, string brand, double price, string displayType, string features);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        ElectronicWatch::save(out);
        out << " " << features;
    }

    void load(istream& in) override {
        ElectronicWatch::load(in);
        in >> features;
    }

    string getWatchType() const override {
        return type;
    }
private:
    string features;
    string type = "SmartWatch";
};