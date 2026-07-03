#ifndef LAB3_V12_FACTORY_H
#define LAB3_V12_FACTORY_H

#include <iostream>

using namespace std;

class Factory {
protected:
    string factoryName;
    int employees;
public:
    Factory(string factoryName = "", int e = 1);

    ~Factory();

    void setEmpoyees(int emloyees);

    int getEmpoyees() const;

    void setFactoryName(string name);

    string getFactoryName() const;

    friend ostream &operator<<(ostream &out, Factory &object);
};

#endif //LAB3_V12_FACTORY_H