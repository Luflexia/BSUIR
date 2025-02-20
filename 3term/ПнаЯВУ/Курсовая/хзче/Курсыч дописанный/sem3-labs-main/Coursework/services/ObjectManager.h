//
// Created by Egor on 12/8/2022.
//

#ifndef COURSEWORK_OBJECTMANAGER_H
#define COURSEWORK_OBJECTMANAGER_H

#include <string>
#include <fstream>
#include <windows.h>
#include "Exception.h"
#include "Input.h"

using namespace std;

class ObjectManager {
    Input in;
public:
    string requestPhone() {
        return in.inputMobile();
    }

    string requestEmail() {
        return in.inputEmail();
    }

    float requestCost() {
        return in.inputFloat(100, 2000000000, "Введите цену: ");
    }

    double requestSqr() {
        return in.inputDouble(10, 100000000, "Введите общую площадь: ");
    }

    string requestAddr() {
        return in.inputString("Введите адрес:");
    }

    bool requestPond() {
        return in.inputBool("Есть водоёмы?");
    }

    bool requestPlats() {
        return in.inputBool("Есть деревья/кустарники?");
    }

    bool requestCommun() {
        return in.inputBool("Проведены ли коммуникации?");
    }

    int requestFloor(bool isFlat = false) {
        if (isFlat)
            return in.inputInt(1, 100, "Введите на каком этаже?");
        return in.inputInt(1, 5, "Сколько этажей?");
    }

    int requestRooms() {
        return in.inputInt(1, 100, "Сколько комнат?");
    }

    int requestParking() {
        return in.inputInt(1, 100, "Сколько парковочных мест?");
    }

    bool requestSuiFCons() {
        return in.inputBool("Пригодно ли для строительства?");
    }

    bool requestSuiFFarm() {
        return in.inputBool("Пригодно ли для фермерства?");
    }

    bool requestHaveBalcony() {
        return in.inputBool("Есть ли балкон?");
    }

    int requestType(bool isParking = false) {
        if (isParking) {
            string msg = "1. Машино место\n"
                         "2. Бокс\n"
                         "3. Гараж\n"
                         "4. Другое\n"
                         "Выберите тип:";
            return in.inputInt(1, 4, msg);
        }
        string msg = "1. Офис\n"
                     "2. Магазин, торговое помещение\n"
                     "3. Склад\n"
                     "4. Другое\n"
                     "Выберите тип:";
        return in.inputInt(1, 4, msg);
    }
};

#endif //COURSEWORK_OBJECTMANAGER_H