#ifndef COURSEWORK_COMMERCIAL_H
#define COURSEWORK_COMMERCIAL_H

#include "Immovable.h"
#include "Input.h"

class Commercial : public Immovable {
    int type;
public:
    Commercial(unsigned int id, const string& phone, const string& email, float cost, double square,
        const string& address, bool actuality, int type = -1);

    ~Commercial();

    void printInfo() override;

    friend ostream& operator<<(ostream& out, Commercial& myClass);

    void setType(int type);

    string printType();

    int getType();
};

#endif //COURSEWORK_COMMERCIAL_H