//
// Created by Egor on 11/27/2022.
//

#ifndef COURSEWORK_PARKING_H
#define COURSEWORK_PARKING_H

#include "../Immovable.h"

/**
 - Тип (, , )
 - Наличие ямы (Только для Тип: Гараж)
 */

class Parking : public Immovable{
    int type;
public:
    Parking(unsigned int id, const string &phonePrimary, const string &email, float cost, double square,
            const string &address, bool actuality, int type = 3) : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
        setType(type);
    }

    void printInfo() override {
        cout << "  Информация о парковке:" << endl
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
        string msg = "1. Машино место\n"
                     "2. Бокс\n"
                     "3. Гараж\n"
                     "4. Другое\n"
                     "Выберите тип:";
        int type = in.inputInt(1, 4, msg);
        setType(type);
        return;
    }

    void setType(int type){
        this->type = type;
    }

    string printType(){
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

    int getType(){
        return type;
    }
};

#endif //COURSEWORK_PARKING_H
