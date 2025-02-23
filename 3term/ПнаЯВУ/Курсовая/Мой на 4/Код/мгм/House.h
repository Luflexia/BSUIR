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
        bool pond, bool plants, bool communications, int floors = 1, int rooms = 4, int parkingSpaces = 2)
        : Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {
        setFloors(floors);
        setRooms(rooms);
        setParking(parkingSpaces);
    }

    ~House() {}

    void printInfo() override {
        cout << "  ���������� � ����/��������:" << endl
            << "ID: " << getId();
        if (!getActuality()) {
            cout << endl << "������������ ���� ������/�������." << endl << lineStr;
            return;
        }

        cout << endl << "���������: " << printCost() << endl
            << "�����: " << getAddress() << endl
            << "����� �������: " << printSquare() << endl

            << "������: " << getRooms() << endl
            << "������: " << getFloors() << endl
            << "����������� ����: " << getParkingSpace() << endl

            << "�������      - " << boolToString(pond()) << endl
            << "��������     - " << boolToString(plants()) << endl
            << getContact() << endl << lineStr;
    }

    friend ostream& operator<<(ostream& out, House& myClass) {
        out << "class house\n"
            << "id " << myClass.getId() << '\n'
            << "phone " << myClass.getMobile() << '\n'
            << "email " << myClass.getEmail() << '\n'
            << "cost " << myClass.getCost() << '\n'
            << "sqr " << myClass.getSquare() << '\n'
            << "addr " << myClass.getAddress() << '\n'
            << "actual " << myClass.getActuality() << '\n'
            << "pond " << myClass.pond() << '\n'
            << "plant " << myClass.plants() << '\n'
            << "commun " << myClass.communications() << '\n'
            << "parking " << myClass.getParkingSpace() << '\n'
            << "rooms " << myClass.getRooms() << '\n'
            << "floor " << myClass.getFloors() << '\n';
        return out;
    };

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
