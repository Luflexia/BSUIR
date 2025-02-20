#ifndef COURSEWORK_STORAGE_H
#define COURSEWORK_STORAGE_H

#include <string>
//#include <list>
#include <fstream>
#include "Exception.h"
#include "Input.h"
#include "ObjectManager.h"
#include "House.h"
#include "Empty.h"
#include "Flat.h"
#include "Parking.h"
#include "Commercial.h"

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
