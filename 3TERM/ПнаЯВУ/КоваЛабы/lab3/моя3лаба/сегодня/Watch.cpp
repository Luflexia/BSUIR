// Watch.cpp
#include "Watch.h"

int Watch::count = 0;

Watch::Watch(std::string type, int id, string brand, double price) : type(type), id(++count), brand(brand), price(price) {}

Watch::~Watch() {}

void Watch::display() const {
    cout << setw(S) << id;//<< "Watch #" 
    cout << setw(S) << brand;//<< "Brand: "
    cout << setw(S) << price;//<< "Price: $"
}

void Watch::update() {
    cout << "Enter brand: ";
    cin >> brand;
    while (true) {
        cout << "Enter price: ";
        try {
            cin >> price;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InvalidInputException("Invalid input. Please enter a valid value.");
            }

            if (price < 0.0) {
                throw InvalidInputException("Price cannot be negative. Please enter a valid value.");
            }

            break; 
        }
        catch (const InvalidInputException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
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