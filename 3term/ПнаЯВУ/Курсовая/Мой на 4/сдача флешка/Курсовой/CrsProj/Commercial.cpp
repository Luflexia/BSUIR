#include "Commercial.h"

Commercial::Commercial(unsigned int id, const string& phone, const string& email, float cost, double square,
    const string& address, bool actuality, int type) : Immovable(id, phone, email, cost, square,
        address, actuality) {
    setType(type);
}

Commercial::~Commercial() {}

void Commercial::printInfo() {
    cout << "  ���������� � ���������:" << endl
        << "ID: " << getId();
    if (!getActuality()) {
        cout << endl << "������������ ���� ������/�������." << endl << lineStr;
        return;
    }

    cout << endl << "���������: " << printCost() << endl
        << "�����: " << getAddress() << endl
        << "����� �������: " << printSquare() << endl
        << "���: " << printType() << endl
        << getContact() << endl << lineStr;
}

ostream& operator<<(ostream& out, Commercial& myClass) {
    out << "class commercial\n"
        << "id " << myClass.getId() << '\n'
        << "phone " << myClass.getMobile() << '\n'
        << "email " << myClass.getEmail() << '\n'
        << "cost " << myClass.getCost() << '\n'
        << "sqr " << myClass.getSquare() << '\n'
        << "addr " << myClass.getAddress() << '\n'
        << "actual " << myClass.getActuality() << '\n'
        << "type " << myClass.getType() << '\n';
    return out;
}

void Commercial::setType(int type) {
    this->type = type;
}

string Commercial::printType() {
    switch (type) {
    case 1:
        return "����";
    case 2:
        return "�������, �������� ���������";
    case 3:
        return "�����";
    default:
        return "������";
    }
}

int Commercial::getType() {
    return type;
}
