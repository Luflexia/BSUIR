// Watch.cpp
#include "Watch.h"

int Watch::count = 0;

Watch::Watch(int id, string brand, double price) : id(++count), brand(brand), price(price) {}

Watch::~Watch() {}

void Watch::display() const {
    cout << setw(S) << id;//<< "Watch #" 
    cout << setw(S) << brand;//<< "Brand: "
    cout << setw(S) << price;//<< "Price: $"
}

void Watch::update() {
    cout << "Enter brand: ";
    cin >> brand;
    cout << "Enter price: ";
    cin >> price;
}

int Watch::getId() const {
    return id;
}

string Watch::getBrand() const {
    return brand;
}

double Watch::getPrice() const {
    return price;
}

string Watch::getAdditionalInfo() const {
    return "";
}