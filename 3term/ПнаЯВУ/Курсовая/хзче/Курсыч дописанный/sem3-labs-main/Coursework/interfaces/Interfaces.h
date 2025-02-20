//
// Created by Egor on 12/7/2022.
//

#ifndef COURSEWORK_INTERFACES_H
#define COURSEWORK_INTERFACES_H

#include <iostream>
#include <synchapi.h>
#include "../services/Exception.h"
#include "../services/Storage.h"
#include "../services/ObjectManager.h"
#include "../services/Input.h"
#include "../immovables/piece/House.h"
#include "../immovables/piece/Empty.h"
#include "../immovables/flat/Flat.h"
#include "../immovables/parking/Parking.h"
#include "../immovables/commercial/Commercial.h"
#include "../services/ObjectManager.h"

using namespace std;

class Interfaces {
    Storage storage;
    Input in;
    ObjectManager objectManager;

    unsigned int interfaceCode = 0;
    unsigned int resentId = 0;

    const string title = "\tИнформационная система по продаже недвижимости\n";

    template<typename T>
    void printListItem(vector<T> array, unsigned int id);

public:
    void run();

    void printMainMenu();   //  0

    int selectorMainMenu();

    void printFindByID();   //  100

    int selectorFindByID();

    void printViewAll();    //  200

    int selectorViewAll();

                            //  210
    void printViewItem(string className, unsigned int id, bool advancedMode = false);

    int selectorViewItem();

    void printAddNew();     //  300

    void selectorAddNew();

    void actionOnObject(unsigned int id, bool justHide = false);

    template<typename T>
    bool editObject(T &object);

    template<typename T>
    void editImmovable(T &object);

    template<typename T>
    void editPiece(T &object);

    void editHouse(House &object);

    void editEmpty(Empty &object);

    void editFlat(Flat &object);

    void editParking(Parking &object);

    void editCommercial(Commercial &object);

    template<typename T>
    void hideObject(T &object);
};

#endif //COURSEWORK_INTERFACES_H