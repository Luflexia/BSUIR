// Watch.h
#pragma once
#include<iostream>
#include<iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#define S 12

class Watch {
public:
    Watch(int id, std::string brand, double price);
    virtual ~Watch();

    virtual void display() const;
    virtual void update();

    int getId() const;
    std::string getBrand() const;
    double getPrice() const;

    virtual std::string getType() const = 0;
    virtual std::string getAdditionalInfo() const = 0;

private:
    int id;
    std::string brand;
    double price;

    static int count;
};