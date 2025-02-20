// WristSmartWatch.h
#pragma once
#include "SmartWatch.h"

class WristSmartWatch : public SmartWatch {
public:
    WristSmartWatch(std::string type, int id, string brand, double price, string displayType, string features, string strapType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        SmartWatch::save(out);
        out << " " << strapType;
    }

    void load(istream& in) override {
        SmartWatch::load(in);
        in >> strapType;
    }

    string getWatchType() const override {
        return type;
    }
private:
    string strapType;
    string type = "WristSmartWatch";
};