//
// Created by Egor on 11/27/2022.
//

#ifndef COURSEWORK_PARKING_H
#define COURSEWORK_PARKING_H

#include "../Immovable.h"

class Parking : public Immovable {
    int type;
public:
    Parking(unsigned int id, const string &phonePrimary, const string &email, float cost, double square,
            const string &address, bool actuality, int type = 3) : Immovable(id, phonePrimary, email, cost, square,
                                                                             address, actuality) {
        setType(type);
    }

    ~Parking() override = default;

    void printInfo() override {
        cout << "  Информация о парковке:" << endl
             << "ID: " << getId();
        if (!getActuality()) {
            cout << endl << "Недвижимость была скрыта/удалена." << endl << lineStr;
            return;
        }

        cout << endl << "Стоимость: " << printCost() << endl
             << "Адрес: " << getAddress() << endl
             << "Общая площадь: " << printSquare() << endl
             << "Тип: " << printType() << endl
             << getContact() << endl << lineStr;
    }

    friend ostream &operator<<(ostream &out, Parking &myClass) {
        out << "class parking\n"
            << "id " << myClass.getId() << '\n'
            << "phone " << myClass.getMobile() << '\n'
            << "email " << myClass.getEmail() << '\n'
            << "cost " << myClass.getCost() << '\n'
            << "sqr " << myClass.getSquare() << '\n'
            << "addr " << myClass.getAddress() << '\n'
            << "actual " << myClass.getActuality() << '\n'
            << "type " << myClass.getType() << '\n';
        return out;
    };

    void setType(int type) {
        this->type = type;
    }

    string printType() {
        switch (type) {
            case 1:
                return "Машино место";
            case 2:
                return "Бокс";
            case 3:
                return "Гараж";
            default:
                return "Другое";
        }
    }

    int getType() {
        return type;
    }
};

#endif //COURSEWORK_PARKING_H