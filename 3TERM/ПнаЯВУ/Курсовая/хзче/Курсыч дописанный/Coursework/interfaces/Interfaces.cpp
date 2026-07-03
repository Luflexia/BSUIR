//
// Created by Egor on 12/7/2022.
//

#include <cstdlib>
#include "Interfaces.h"

void Interfaces::printMainMenu() {
    system("CLS");
    cout << title;
    cout << mainMenu;
    selectorMainMenu();
}

void Interfaces::printFindByID() {
    system("CLS");
    cout << title;
    cout << findByID;
    selectorFindByID();
}

void Interfaces::printViewAll() {

    // while(list) ?

    cout << "Доступные действия:" << endl
    << "1. Поиск по ID" << endl
    << "0. Назад" << endl;
    selectorViewAll();
}

void Interfaces::printAddNew() {
    system("CLS");
    const string msg = "Типы недвижимости:\n"
                       "1. Дом, Коттедж\n"
                       "2. Пустой участок\n"
                       "3. Квартира"
                       "4. Гаражи и стоянки\n"
                       "5. Коммерческая\n"
                       "0. Назад\n";
    cout << msg;
    selectorAddNew();
}


void Interfaces::exit() {
    // Saving data in file;
    ::exit(EXIT_SUCCESS);
}

void Interfaces::selectorMainMenu() {
    switch (in.inputInt(0, 3, "Выберите пункт:")) {
        case 1:
            printFindByID();
            break;
        case 2:
            printViewAll();
            break;
        case 3:
            printAddNew();
            break;
        case 0:
            exit();
            break;
        default:
            throw Exception("Не существующее значение.", -1);
    };
}