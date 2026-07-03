#include "Storage.h"

Storage::Storage() {
    try {
        uploadAppFile();        //  get actual ID
        loadAll();              //  load all objects
    }
    catch (Exception exception) {
        exception.what();
    }
    catch (...) {
        unexpected();
    }
}

void Storage::uploadAppFile() {
    ifstream in;
    in.open(filepath);
    if (!in.is_open()) {
        saveAppFile();
        throw Exception("AppFile не найден.\nAppFile был пересоздан.", 2);
    }

    in >> freeId;

    in.close();
}

void Storage::saveAppFile() {
    ofstream out;
    out.open(filepath);
    if (!out.is_open())
        throw Exception("Невозможно открыть AppFile для записи.", 0);

    out << freeId;

    out.close();
}

template<class T>
void Storage::upload(T className) {
    const string path = to_string(className.getId()) + ".txt";
    ofstream out;
    out.open(path);
    if (!out.is_open())
        throw Exception("Невозможно открыть файл для записи.", 0);

    out << className;

    out.close();
}

void Storage::load(unsigned int id) {
    const string path = to_string(id) + ".txt";
    ifstream in;
    in.open(path);
    if (!in.is_open())
        throw Exception("Невозможно открыть файл для чтения.", 0);

    //  FLAGS
    string classType;
    //  all
    string phone;
    string email;
    float cost = -1;
    double sqr = -1;
    string addr;
    bool actual = false;
    //  piece
    bool pond = false;
    bool plant = false;
    bool commun = false;
    //  empty
    bool suifcons = false;
    bool suiffarm = false;
    //  house
    int parking = -1;
    //  flat & house
    int rooms = -1;
    int floor = -1;
    //  flat
    bool balcony = false;
    //  commercial & parking
    int type = -1;

    //  Set flags
    string str;
    while (getline(in, str)) {
        string name = str.substr(0, str.find(' '));
        string value = str.substr(str.find(' ') + 1, str.find('\n'));
        if (name == "class")
            classType = value;
        else if (name == "phone")
            phone = value;
        else if (name == "email")
            email = value;
        else if (name == "cost")
            cost = stof(value);
        else if (name == "sqr")
            sqr = stod(value);
        else if (name == "addr")
            addr = value;
        else if (name == "actual")
            actual = stob(value);
        else if (name == "pond")
            pond = stob(value);
        else if (name == "plant")
            plant = stob(value);
        else if (name == "commun")
            commun = stob(value);
        else if (name == "suifcons")
            suifcons = stob(value);
        else if (name == "suiffarm")
            suiffarm = stob(value);
        else if (name == "parking")
            parking = stoi(value);
        else if (name == "rooms")
            rooms = stoi(value);
        else if (name == "floor")
            floor = stoi(value);
        else if (name == "balcony")
            balcony = stob(value);
        else if (name == "type")
            type = stoi(value);
    }

    //  Create object & push it
    if (classType == "house") {
        House house(id, phone, email, cost, sqr, addr, actual, pond, plant, commun,
            floor, rooms, parking);
        listHouses.push_back(house);
    }
    else if (classType == "empty") {
        Empty empty(id, phone, email, cost, sqr, addr, actual, pond, plant, commun, suifcons, suiffarm);
        listEmpty.push_back(empty);
    }
    else if (classType == "flat") {
        Flat flat(id, phone, email, cost, sqr, addr, actual, rooms, floor, balcony);
        listFlat.push_back(flat);
    }
    else if (classType == "parking") {
        Parking parking(id, phone, email, cost, sqr, addr, actual, type);
        listParking.push_back(parking);
    }
    else if (classType == "commercial") {
        Commercial commercial(id, phone, email, cost, sqr, addr, actual, type);
        listCommercial.push_back(commercial);
    }
    in.close();
}

void Storage::loadAll() {
    for (size_t i = 0; i < freeId; ++i) {
        load(i);            //  load objects
        Sleep(75);
    }
}

template<typename T>
void Storage::addClass(T className) {
    try {
        upload(className);
        load(getFreeId() - 1);
    }
    catch (Exception exception) {
        exception.what();
    }
    catch (...) {
        unexpected();
    }
}

void Storage::addHouse() {
    ObjectManager manager;
    House house(requestId(), manager.requestPhone(), manager.requestEmail(), manager.requestCost(),
        manager.requestSqr(), manager.requestAddr(), true, manager.requestPond(), manager.requestPlats(),
        manager.requestCommun(), manager.requestFloor(), manager.requestRooms(), manager.requestParking());
    addClass(house);
}

void Storage::addEmpty() {
    ObjectManager manager;
    Empty empty(requestId(), manager.requestPhone(), manager.requestEmail(), manager.requestCost(),
        manager.requestSqr(), manager.requestAddr(), true, manager.requestPond(), manager.requestPlats(),
        manager.requestCommun(), manager.requestSuiFCons(), manager.requestSuiFFarm());
    addClass(empty);
}

void Storage::addFlat() {
    ObjectManager manager;
    Flat flat(requestId(), manager.requestPhone(), manager.requestEmail(), manager.requestCost(),
        manager.requestSqr(), manager.requestAddr(), true, manager.requestRooms(), manager.requestFloor(true),
        manager.requestHaveBalcony());
    addClass(flat);
}

void Storage::addParking() {
    ObjectManager manager;
    Parking parking(requestId(), manager.requestPhone(), manager.requestEmail(), manager.requestCost(),
        manager.requestSqr(), manager.requestAddr(), true, manager.requestType(true));
    addClass(parking);
}

void Storage::addCommercial() {
    ObjectManager manager;
    Commercial commercial(requestId(), manager.requestPhone(), manager.requestEmail(), manager.requestCost(),
        manager.requestSqr(), manager.requestAddr(), true, manager.requestType());
    addClass(commercial);
}

unsigned int Storage::getFreeId() {
    return freeId;
}

unsigned int Storage::requestId() {
    return freeId++;
}

string Storage::identifyObject(unsigned int id) {
    const string path = to_string(id) + ".txt";
    ifstream in;
    in.open(path);
    if (!in.is_open())
        throw Exception("Невозможно открыть файл для чтения.", 0);

    string str;
    string value;
    while (getline(in, str)) {
        string name = str.substr(0, str.find(' '));
        value = str.substr(str.find(' ') + 1, str.find('\n'));

        if (name == "class")
            break;
    }

    in.close();
    return value;
}

bool Storage::stob(string str) {
    int i = stoi(str);
    if (i)
        return true;
    return false;
}