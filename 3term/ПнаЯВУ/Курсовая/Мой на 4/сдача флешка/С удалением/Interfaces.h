#ifndef COURSEWORK_INTERFACES_H
#define COURSEWORK_INTERFACES_H
#include "Exception.h"
#include <iostream>
//#include <list>
#include "Exception.h"
#include "Storage.h"
#include "ObjectManager.h"
#include "Input.h"
#include "House.h"
#include "Empty.h"
#include "Flat.h"
#include "Parking.h"
#include "Commercial.h"
#include "ObjectManager.h"

using namespace std;

class Interfaces {
    Storage storage;
    Input in;
    ObjectManager objectManager;

    unsigned int interfaceCode = 0;
    unsigned int resentId = 0;

    string selectedType;

    const string title = "\t»нформационна€ система по продаже недвижимости\n";

    template<typename T>
    void printListItem(vector<T> array, unsigned int id);


public:
    void run();

    template<typename T>
    void removeObject(T& object);

    //void removeObject(unsigned int id);

    void printMainMenu();   //  0

    int selectorMainMenu();

    void printFindByID();   //  100

    int selectorFindByID();

    void printViewAll();    //  200

    int selectorViewAll();

    void printViewRealEstateMenu();  // Ќовый метод дл€ вывода меню просмотра недвижимости

    int selectorViewRealEstateMenu();  // Ќовый метод дл€ выбора пункта меню просмотра недвижимости

    void viewRealEstateMenu();  // Ќовый метод дл€ обработки меню просмотра недвижимости

    int selectorHideShowObject();

    //  210
    void printViewItem(string className, unsigned int id, bool advancedMode = false);

    int selectorViewItem();

    void printAddNew();     //  300

    void selectorAddNew();

    void actionOnObject(unsigned int id, bool justHide = false);

    template<typename T>
    bool editObject(T& object);

    template<typename T>
    void editImmovable(T& object);

    template<typename T>
    void editPiece(T& object);

    void editHouse(House& object);

    void editEmpty(Empty& object);

    void editFlat(Flat& object);

    void editParking(Parking& object);

    void editCommercial(Commercial& object);

    //template<typename T>
    //void hideObject(T& object);
};

#endif //COURSEWORK_INTERFACES_H
