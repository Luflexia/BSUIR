#include "House.h"

House::House(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
    const string& address, bool actuality,
    bool pond, bool plants, bool communications, int floors, int rooms, int parkingSpaces)
    : Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {
    setFloors(floors);
    setRooms(rooms);
    setParking(parkingSpaces);
}

House::~House() {}

void House::printInfo() {
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

ostream& operator<<(ostream& out, House& myClass) {
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
}

void House::setFloors(int value) {
    floors = value;
}

int House::getFloors() {
    return floors;
}

void House::setRooms(int value) {
    rooms = value;
}

int House::getRooms() {
    return rooms;
}

void House::setParking(int value) {
    parkingSpaces = value;
}

int House::getParkingSpace() {
    return parkingSpaces;
}
