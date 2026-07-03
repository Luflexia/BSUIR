// ElectronicWatch.h
#pragma once
#include "Watch.h"

class ElectronicWatch : public Watch {
public:
    ElectronicWatch(string type, int id, string brand, double price, string displayType);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        Watch::save(out);
        out << " " << displayType;
    }

    void load(istream& in) override {
        Watch::load(in);
        in >> displayType;
    }

    string getWatchType() const override {
        return type;
    }
private:
    string displayType;
    string type = "ElectronicWatch";
};