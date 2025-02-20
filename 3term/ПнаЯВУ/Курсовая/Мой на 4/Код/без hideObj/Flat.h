#ifndef COURSEWORK_FLAT_H
#define COURSEWORK_FLAT_H

#include "Immovable.h"

class Flat : public Immovable {
    int rooms;
    int floor;
    bool haveBalcony;
public:
    Flat(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality, int rooms = 2, int floor = 5, bool haveBalcony = true)
        : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
        setRooms(rooms);
        setFloor(floor);
        isHaveBalcony(haveBalcony);
    }

    ~Flat() {}

    void printInfo() override {
        cout << "  ���������� � ��������:" << endl
            << "ID: " << getId();
        if (!getActuality()) {
            cout << endl << "������������ ���� ������/�������." << endl << lineStr;
            return;
        }

        cout << endl << "���������: " << printCost() << endl
            << "�����: " << getAddress() << endl
            << "����� �������: " << printSquare() << endl

            << "������: " << getRooms() << endl
            << "����: " << getFloor() << endl
            << "���� ������: " << boolToString(isHaveBalcony()) << endl
            << getContact() << endl << lineStr;
    }

    friend ostream& operator<<(ostream& out, Flat& myClass) {
        out << "class flat\n"
            << "id " << myClass.getId() << '\n'
            << "phone " << myClass.getMobile() << '\n'
            << "email " << myClass.getEmail() << '\n'
            << "cost " << myClass.getCost() << '\n'
            << "sqr " << myClass.getSquare() << '\n'
            << "addr " << myClass.getAddress() << '\n'
            << "actual " << myClass.getActuality() << '\n'
            << "rooms " << myClass.getRooms() << '\n'
            << "floor " << myClass.getFloor() << '\n'
            << "balcony " << myClass.isHaveBalcony() << '\n';
        return out;
    };

    void setFloor(int value) {
        floor = value;
    }

    int getFloor() {
        return floor;
    }

    void setRooms(int value) {
        rooms = value;
    }

    int getRooms() {
        return rooms;
    }

    void isHaveBalcony(bool have) {
        haveBalcony = have;
    }

    bool isHaveBalcony() {
        return haveBalcony;
    }
};

#endif //COURSEWORK_FLAT_H
