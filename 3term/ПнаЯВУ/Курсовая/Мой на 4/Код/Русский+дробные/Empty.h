#ifndef COURSEWORK_EMPTY_H
#define COURSEWORK_EMPTY_H

#include "Piece.h"

class Empty : public Piece {
    bool suitableForConstruction;
    bool suitableForFarming;
public:

    Empty(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality, bool pond, bool plants, bool communications,
        bool suitableForConstruction = true, bool suitableForFarming = true) :
        Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {
        setConstruction(suitableForConstruction);
        setFarming(suitableForFarming);
    }

    ~Empty() {}

    void printInfo() override {
        cout << "  Информация о участке:" << endl
            << "ID: " << getId();
        if (!getActuality()) {
            cout << endl << "Недвижимость была скрыта/удалена." << endl << lineStr;
            return;
        }

        cout << endl << "Стоимость: " << printCost() << endl
            << "Адрес: " << getAddress() << endl
            << "Общая площадь: " << printSquare() << endl
            << "Пригодно для строительства: " << boolToString(suitableConstruction()) << endl
            << "Пригодно для фермерства:    " << boolToString(suitableFarming()) << endl
            << "Водоемы:      " << boolToString(pond()) << endl
            << "Растения:     " << boolToString(plants()) << endl
            << "Коммуникации: " << boolToString(communications()) << endl
            << getContact() << endl << lineStr;
    }

    friend ostream& operator<<(ostream& out, Empty& myClass) {
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
    };

    void setConstruction(bool suitable) {
        suitableForConstruction = suitable;
    }

    bool suitableConstruction() {
        return suitableForConstruction;
    }

    void setFarming(bool suitable) {
        suitableForFarming = suitable;
    }

    bool suitableFarming() {
        return suitableForFarming;
    }
};

#endif //COURSEWORK_EMPTY_H
