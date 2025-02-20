//
// Created by Egor on 12/6/2022.
//

#ifndef COURSEWORK_HOUSE_H
#define COURSEWORK_HOUSE_H

#include "Piece.h"

class House : public Piece {
    int floors;
    int rooms;
    int parkingSpaces;
public:
    House(unsigned int id, const string &phonePrimary, const string &email, float cost, double square, const string &address, bool actuality,
          bool pond, bool plants, bool communications, int floors = 1, int rooms = 4, int parkingSpaces = 2)
            : Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {

        setFloors(floors);
        setRooms(rooms);
        setParking(parkingSpaces);
    }

    void printInfo() override {
        cout << "  Информация о доме/коттедже:" << endl
             << "ID: " << getId();
        if (!getActuality())
            cout << " [Не актуально]" << endl;

        cout << endl << "Стоимость: " << printCost() << endl
             << "Адрес: " << getAddress() << endl
             << "Общая площадь: " << printSquare() << endl

             << "Комнат: " << getRooms() << endl
             << "Этажей: " << getFloors() << endl
             << "Парковочный мест: " << getParkingSpace() << endl

             << "Водоемы      - " << boolToString(pond()) << endl
             << "Растения     - " << boolToString(plants()) << endl
             << getContact() << endl << lineStr;
    }

    void setFloors(int value) {
        floors = value;
    }

    int getFloors() {
        return floors;
    }

    void setRooms(int value) {
        rooms = value;
    }

    int getRooms() {
        return rooms;
    }

    void setParking(int value) {
        parkingSpaces = value;
    }

    int getParkingSpace() {
        return parkingSpaces;
    }
};

#endif //COURSEWORK_HOUSE_H