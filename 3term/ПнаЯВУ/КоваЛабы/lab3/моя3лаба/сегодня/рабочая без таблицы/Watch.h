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
    Watch(int id, string brand, double price);
    virtual ~Watch();

    virtual void display() const;
    virtual void update();

    int getId() const;
    string getBrand() const;
    double getPrice() const;

    virtual string getAdditionalInfo() const = 0;

private:
    int id;
    string brand;
    double price;

    static int count;
};