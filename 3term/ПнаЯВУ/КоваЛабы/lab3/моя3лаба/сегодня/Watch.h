// Watch.h
#pragma once
#include<iostream>
#include<iomanip>
#include <string>
#include "MyList.h"
#include <algorithm>
#include "Exceptions.h"
using namespace std;
#define S 12

class Watch {
public:
    Watch(std::string type, int id, string brand, double price);
    virtual ~Watch();

    virtual void display() const;
    virtual void update();

    int getId() const;
    string getBrand() const;
    double getPrice() const;

    virtual string getAdditionalInfo() const = 0;

    virtual void load(istream& in) {
        in >> type >> id >> brand >> price;
    }
    virtual void save(ostream& out) const {
        out << type << id << " " << brand << " " << price;
    }

    virtual string getWatchType() const {
        return type;
    }

private:
    int id;
    string brand;
    double price;
    string type;
    static int count;
};