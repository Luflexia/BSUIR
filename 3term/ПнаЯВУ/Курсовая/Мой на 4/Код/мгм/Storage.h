#ifndef COURSEWORK_STORAGE_H
#define COURSEWORK_STORAGE_H

#include <string>
//#include <vector>
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

    vector<House> vectorHouses;
    vector<Empty> vectorEmpty;
    vector<Flat> vectorFlat;
    vector<Parking> vectorParking;
    vector<Commercial> vectorCommercial;

    void saveAppFile();

    //  from class to file
    template<class T>
    void upload(T className);

    //  from file to vector
    void load(unsigned int id);

    //  create & add to file & to vector
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
