//
// Created by Egor on 12/6/2022.
//

#include "MainMenu.h"
#include "../services/Exception.h"
#include "../services/Input.h"
#include "AddNew.h"

void MainMenu::print(){
    system("CLS");
    cout << title;
    cout << menu;
}

void MainMenu::exit() {
    // Saving data in file;
    ::exit(EXIT_SUCCESS);
}

void MainMenu::selector() {
    Input in;
    int choice; // input int func
    switch (in.inputInt(0, 3, "Выберите пункт:")) {
        case 1:
            cout << "Пункт #1" << endl;
            break;
        case 2:
            cout << "Пункт #2" << endl;
            break;
        case 3:
            cout << "Пункт #3" << endl;
            AddNew();
            break;
        case 0:
            exit();
            break;
        default:
            throw Exception("Не существующее значение.",-1);
    };
}