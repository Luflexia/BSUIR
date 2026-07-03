#ifndef COURSEWORK_HOUSE_H
#define COURSEWORK_HOUSE_H

#include "Piece.h"

class House : public Piece {
    int floors;
    int rooms;
    int parkingSpaces;
public:
    House(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality,
        bool pond, bool plants, bool communications, int floors = 1, int rooms = 4, int parkingSpaces = 2);

    ~House();

    void printInfo() override;

    friend ostream& operator<<(ostream& out, House& myClass);

    void setFloors(int value);

    int getFloors();

    void setRooms(int value);

    int getRooms();

    void setParking(int value);

    int getParkingSpace();
};

#endif //COURSEWORK_HOUSE_H
