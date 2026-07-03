//
// Created by Egor on 11/27/2022.
//

#ifndef COURSEWORK_COMMERCIAL_H
#define COURSEWORK_COMMERCIAL_H

#include "../Immovable.h"
#include "../../services/Input.h"

class Commercial : public Immovable {
    int type;
public:
    Commercial(unsigned int id, const string &phonePrimary, const string &email, float cost, double square,
               const string &address, bool actuality, int type = -1) : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
        setType(type);
    }

    void printInfo() override {
        cout << "  Информация о помещение:" << endl
             << "ID: " << getId();
        if (!getActuality())
            cout << " [Не актуально]" << endl;

        cout << endl << "Стоимость: " << printCost() << endl
             << "Адрес: " << getAddress() << endl
             << "Общая площадь: " << printSquare() << endl
             << "Тип: " << printType() << endl
             << getContact() << endl << lineStr;
    }

    void inputType() {
        Input in;
        string msg = "1. Офис\n"
                     "2. Магазин, торговое помещение\n"
                     "3. Склад\n"
                     "4. Другое\n"
                     "Выберите тип:";
        int type = in.inputInt(1, 4, msg);
        setType(type);
        return;
    }

    void setType(int type) {
        this->type = type;
    }

    string printType() {
        switch (type) {
            case 1:
                return "Офис";
            case 2:
                return "Магазин, торговое помещение";
            case 3:
                return "Склад";
            default:
                return "Другое";
        }
    }

    int getType() {
        return type;
    }
};

#endif //COURSEWORK_COMMERCIAL_H