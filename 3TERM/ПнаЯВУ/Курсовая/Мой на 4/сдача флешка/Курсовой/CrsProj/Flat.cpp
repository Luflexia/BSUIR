#include "Flat.h"

Flat::Flat(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
    const string& address, bool actuality, int rooms, int floor, bool haveBalcony)
    : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
    setRooms(rooms);
    setFloor(floor);
    isHaveBalcony(haveBalcony);
}

Flat::~Flat() {}

void Flat::printInfo() {
    cout << "  Информация о квартире:" << endl
        << "ID: " << getId();
    if (!getActuality()) {
        cout << endl << "Недвижимость была скрыта/удалена." << endl << lineStr;
        return;
    }

    cout << endl << "Стоимость: " << printCost() << endl
        << "Адрес: " << getAddress() << endl
        << "Общая площадь: " << printSquare() << endl
        << "Комнат: " << getRooms() << endl
        << "Этаж: " << getFloor() << endl
        << "Есть балкон: " << boolToString(isHaveBalcony()) << endl
        << getContact() << endl << lineStr;
}

ostream& operator<<(ostream& out, Flat& myClass) {
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
}

void Flat::setFloor(int value) {
    floor = value;
}

int Flat::getFloor() {
    return floor;
}

void Flat::setRooms(int value) {
    rooms = value;
}

int Flat::getRooms() {
    return rooms;
}

void Flat::isHaveBalcony(bool have) {
    haveBalcony = have;
}

bool Flat::isHaveBalcony() {
    return haveBalcony;
}
