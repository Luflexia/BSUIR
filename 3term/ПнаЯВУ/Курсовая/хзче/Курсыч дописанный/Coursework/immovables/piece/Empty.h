//
// Created by Egor on 12/7/2022.
//

#ifndef COURSEWORK_EMPTY_H
#define COURSEWORK_EMPTY_H

#include "Piece.h"

class Empty : public Piece {
    bool suitableForConstruction;
    bool suitableForFarming;
public:

    Empty(unsigned int id, const string &phonePrimary, const string &email, float cost, double square,
          const string &address, bool actuality, bool pond, bool plants, bool communications,
          bool suitableForConstruction = true, bool suitableForFarming = true)
            : Piece(id, phonePrimary, email, cost, square, address, actuality, pond, plants, communications) {
        setConstruction(suitableForConstruction);
        setFarming(suitableForFarming);
    }

    void printInfo() override {
        cout << "  Информация о участке:" << endl
             << "ID: " << getId();
        if (!getActuality())
            cout << " [Не актуально]" << endl;

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
