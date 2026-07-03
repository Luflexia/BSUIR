//
// Created by Egor on 12/4/2022.
//

#ifndef COURSEWORK_STORAGE_H
#define COURSEWORK_STORAGE_H

#include <string>
#include <list>
#include <fstream>
#include "../services/Exception.h"
#include "../services/Input.h"
#include "../services/ObjectManager.h"
#include "../immovables/piece/House.h"
#include "../immovables/piece/Empty.h"
#include "../immovables/flat/Flat.h"
#include "../immovables/parking/Parking.h"
#include "../immovables/commercial/Commercial.h"

class Storage {
    string filepath = "app.txt";
    unsigned int freeId = 0;

    //  string to bool
    bool stob(string str);

    void uploadAppFile();

    //  load all exists objects
    void loadAll();

    template<typename T>
    void addClass(T className);

public:
    Storage();  //  get actual id

    vector<House> listHouses;
    vector<Empty> listEmpty;
    vector<Flat> listFlat;
    vector<Parking> listParking;
    vector<Commercial> listCommercial;

    void saveAppFile();

    //  from class to file
    template<class T>
    void upload(T className);

    //  from file to list
    void load(unsigned int id);

    //  create & add to file & to list
    void addHouse();

    void addEmpty();

    void addFlat();

    void addParking();

    void addCommercial();

    unsigned int getFreeId();

    unsigned int requestId();

    //  from ID to classType
    string identifyObject(unsigned int id);
};

#endif //COURSEWORK_STORAGE_H