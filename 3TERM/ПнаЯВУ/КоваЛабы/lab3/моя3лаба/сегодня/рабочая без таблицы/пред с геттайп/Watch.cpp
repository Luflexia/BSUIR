// Watch.cpp
#include "Watch.h"

int Watch::count = 0;

Watch::Watch(int id, std::string brand, double price) : id(++count), brand(brand), price(price) {}

Watch::~Watch() {}

void Watch::display() const {
    std::cout << setw(S) << id;//<< "Watch #" 
    std::cout << setw(S) << brand;//<< "Brand: "
    std::cout << setw(S) << price;//<< "Price: $"
}

void Watch::update() {
    std::cout << "Enter brand: ";
    std::cin >> brand;
    std::cout << "Enter price: ";
    std::cin >> price;
}

int Watch::getId() const {
    return id;
}

std::string Watch::getBrand() const {
    return brand;
}

double Watch::getPrice() const {
    return price;
}

std::string Watch::getType() const {
    return "Generic Watch";
}

std::string Watch::getAdditionalInfo() const {
    return "";
}