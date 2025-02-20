#ifndef COURSEWORK_PARKING_H
#define COURSEWORK_PARKING_H

#include "Immovable.h"

class Parking : public Immovable {
    int type;

public:
    Parking(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality, int type = 3);

    ~Parking();

    void printInfo() override;

    friend ostream& operator<<(ostream& out, Parking& myClass);

    void setType(int type);

    string printType();

    int getType();
};

#endif //COURSEWORK_PARKING_H
