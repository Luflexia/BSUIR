// TableClock.h
#pragma once
#include "MechanicalWatch.h"

class TableClock : public MechanicalWatch {
public:
    TableClock(std::string type, int id, string brand, double price, string mechanism, bool hasAlarm);
    void display() const override;
    void update() override;

    string getAdditionalInfo() const override;

    void save(ostream& out) const override {
        TableClock::save(out);
        out << " " << hasAlarm;
    }

    void load(istream& in) override {
        TableClock::load(in);
        in >> hasAlarm;
    }

    string getWatchType() const override {
        return type;
    }

private:
    bool hasAlarm;
    string type = "TableClock";
};