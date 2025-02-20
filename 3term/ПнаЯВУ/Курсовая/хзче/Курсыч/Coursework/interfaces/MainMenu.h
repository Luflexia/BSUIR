//
// Created by Egor on 12/6/2022.
//

#ifndef COURSEWORK_MAINMENU_H
#define COURSEWORK_MAINMENU_H

#include <string>
#include <iostream>

using namespace std;

class MainMenu {
    string title = "\tИнформационная система по продаже недвижимости\n";
    string menu = "1. Поиск по ID\n"
                  "2. Просмотр всей доступной недвижимость\n"
                  "3. Добавить недвижимость\n"
                  "0. Выход\n";

    void exit();

public:
    void print();

    void selector();
};

#endif //COURSEWORK_MAINMENU_H