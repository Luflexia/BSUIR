#include <iostream>

using namespace std;

class Factory {
protected:
    string factoryName;
    int employees;
    double area;
public:
    Factory(string fn = "", int e = 1, double a = 1) {
        factoryName = fn;
        employees = e;
        area = a;
    }

//  Virtual Method
    virtual void printData() = 0;

    virtual double density() = 0;

    virtual ~Factory() { factoryName.clear(); };

};

class Bicycle : public Factory {
protected:
    string brand;
public:
    Bicycle(string fn = "Kona Bicycle Company", int e = 2100, double a = 35000, string b = "Kona") {
        factoryName = fn;
        employees = e;
        area = a;
        brand = b;
    }

    void printData() override {
        cout << "Characteristics:"
             << endl << "\tFactory name: " << factoryName
             << endl << "\tEmployees: " << employees
             << endl << "\tFactory area: " << area
             << endl << "\tBrand: " << brand
             << endl;
    }

    double density() override {
        return employees / area;
    };

    ~Bicycle() override {
        factoryName.clear();
        brand.clear();
    };
};

class Moped : public Bicycle {
protected:
    double maxSpeed;
public:
    Moped(string fn = "Vespa Company", int e = 13500, double a = 89000, string b = "Vespa", double ms = 50) {
        factoryName = fn;
        employees = e;
        area = a;
        brand = b;
        maxSpeed = ms;
    }

    void printData() override {
        cout << "Characteristics:"
             << endl << "\tFactory name: " << factoryName
             << endl << "\tEmployees: " << employees
             << endl << "\tFactory area: " << area
             << endl << "\tBrand: " << brand
             << endl << "\tMax Speed: " << maxSpeed << " km/h"
             << endl;
    }

    double density() override {
        return employees / area;
    };

    ~Moped() override {
        factoryName.clear();
        brand.clear();
    };
};

class Scooter : public Factory {
protected:
    double weight;
public:
    Scooter(string fn = "Gilera", int e = 16500, double a = 45000, double w = 13.7) {
        factoryName = fn;
        employees = e;
        area = a;
        weight = w;
    }

    void printData() override {
        cout << "Characteristics:"
             << endl << "\tFactory name: " << factoryName
             << endl << "\tEmployees: " << employees
             << endl << "\tFactory area: " << area
             << endl << "\tWeight: " << weight << " kg"
             << endl;
    }

    double density() override {
        return employees / area;
    };

    ~Scooter() override {
        factoryName.clear();
    };
};


int main() {
    Factory** factory = new Factory*[3];
    factory[0] = new Bicycle("Redline Bicycles", 1530, 23000, "BMX");
    factory[1] = new Moped();
    factory[2] = new Scooter();

    for (int i = 0; i < 3; i++) {
        factory[i]->printData();
        cout.precision(2);
        cout << "\tDensity: " << factory[i]->density() << " people per square meter." << endl;
        cout.precision(6);
        delete factory[i];
        cout << endl;
    }
    return 0;
}