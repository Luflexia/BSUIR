//
// Created by Egor on 12/7/2022.
//

#ifndef COURSEWORK_INTERFACES_H
#define COURSEWORK_INTERFACES_H

#include <iostream>
#include "../services/Exception.h"
#include "../services/Input.h"
#include "../immovables/piece/House.h"
#include "../immovables/piece/Empty.h"
#include "../immovables/flat/Flat.h"
#include "../immovables/parking/Parking.h"
#include "../immovables/commercial/Commercial.h"

using namespace std;

class Interfaces {
    Input in;

    const string title = "\tИнформационная система по продаже недвижимости\n";
    const string mainMenu = "1. Поиск по ID\n"
                            "2. Просмотр всей доступной недвижимость\n"
                            "3. Добавить недвижимость\n"
                            "0. Выход\n";
    const string findByID = "Введите ID(-1 что-бы вернуться назад):";

public:
    void printMainMenu();

    void printFindByID();

    void printViewAll();

    void printAddNew();

    void selectorAddNew() {
        int choice; // input int func
        switch (in.inputInt(0, 5, "Выберите тип недвижимости:")) {
            case 1:
                addHouse();
                break;
            case 2:
                addEmpty();
                break;
            case 3:
                addFlat();
                break;
            case 4:
                addParking();
                break;
            case 5:
                addCommercial();
                break;
            case 0:
                printMainMenu();
                break;
            default:
                throw Exception("Не существующее значение.", -1);
        };
    }

    void exit();

    void selectorMainMenu();

    void selectorFindByID() {
        int choice; // input int func
        switch (in.inputInt(-1, 65000)) {
            case -1:
                exit();
                break;
            default:
                throw Exception("Не существующее значение.", -1);
        };
    }

    void selectorViewAll() {
        switch (in.inputInt(0, 1, "Выберите действие:")) {
            case 1:
                printFindByID();
                break;
            case 0:
                printMainMenu();
                break;
            default:
                throw Exception("Не существующее значение.", -1);
        };
    }
};

#endif //COURSEWORK_INTERFACES_H