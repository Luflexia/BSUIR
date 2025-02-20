#include "Empty.h"

Empty::Empty(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
    const string& address, bool actuality, bool pond, bool plants, bool communications,
    bool suitableForConstruction, bool suitableForFarming) :
    Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {
    setConstruction(suitableForConstruction);
    setFarming(suitableForFarming);
}

Empty::~Empty() {}

void Empty::printInfo() {
    cout << "  ���������� � �������:" << endl
        << "ID: " << getId();
    if (!getActuality()) {
        cout << endl << "������������ ���� ������/�������." << endl << lineStr;
        return;
    }

    cout << endl << "���������: " << printCost() << endl
        << "�����: " << getAddress() << endl
        << "����� �������: " << printSquare() << endl
        << "�������� ��� �������������: " << boolToString(suitableConstruction()) << endl
        << "�������� ��� ����������:    " << boolToString(suitableFarming()) << endl
        << "�������:      " << boolToString(pond()) << endl
        << "��������:     " << boolToString(plants()) << endl
        << "������������: " << boolToString(communications()) << endl
        << getContact() << endl << lineStr;
}

ostream& operator<<(ostream& out, Empty& myClass) {
    out << "class empty\n"
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
        << "suifcons " << myClass.suitableConstruction() << '\n'
        << "suiffarm " << myClass.suitableFarming() << '\n';
    return out;
}

void Empty::setConstruction(bool suitable) {
    suitableForConstruction = suitable;
}

bool Empty::suitableConstruction() {
    return suitableForConstruction;
}

void Empty::setFarming(bool suitable) {
    suitableForFarming = suitable;
}

bool Empty::suitableFarming() {
    return suitableForFarming;
}
