
#include <cstdlib>
#include "Interfaces.h"

void Interfaces::run() {
    while (true)
        switch (interfaceCode) {
        case 0:
            printMainMenu();
            interfaceCode = selectorMainMenu();
            break;
        case 100:
            printFindByID();
            interfaceCode = selectorFindByID();
            break;
        case 110:
            interfaceCode = selectorViewItem();
            break;
        case 111:
            actionOnObject(resentId);
            interfaceCode = 0;
            cout << "\n�������� ������� ���������." << endl
                << "����������� � ������� ���� ����� 2 �������...";
            Sleep(2000);
            break;
        case 112:
            actionOnObject(resentId, true);
            interfaceCode = 0;
            cout << "\n������������ ���� �������." << endl
                << "����������� � ������� ���� ����� 2 �������...";
            Sleep(2000);
            break;
        case 200:
            printViewAll();
            interfaceCode = selectorViewAll();
            break;
        case 300:
            printAddNew();
            interfaceCode = 0;
            selectorAddNew();
            break;
        case 999:
            try {
                storage.saveAppFile();
            }
            catch (Exception exception) {
                exception.what();
            }
            exit(EXIT_SUCCESS);
        default:
            throw Exception("����������� ��� ����������.", 1);
        }
}

void Interfaces::printMainMenu() {      //  Code 0
    system("CLS");
    cout << title;
    cout << "1. ����� �� ID\n"
        "2. �������� ���� ��������� ������������\n"
        "3. �������� ������������\n"
        "0. �����\n";
}

int Interfaces::selectorMainMenu() {
    switch (in.inputInt(0, 3, "�������� �����:")) {
    case 1:
        return 100;
    case 2:
        return 200;
    case 3:
        return 300;
    case 0:
        return 999;
    default:
        throw Exception("�� ������������ ��������.", -1);
    }
}

void Interfaces::printFindByID() {      //  Code 100
    system("CLS");
    cout << title;
    cout << "������� ID('-1' - ���-�� ��������� �����):";
}

int Interfaces::selectorFindByID() {
    int id = in.inputInt(-1, storage.getFreeId() - 1);
    if (id == -1)
        return 0;
    printViewItem(storage.identifyObject(id), id, true);
    return 110;
}

void Interfaces::printViewAll() {       //  Code 200
    system("CLS");
    cout << title;
    for (unsigned int i = 0; i < storage.getFreeId(); ++i)
        printViewItem(storage.identifyObject(i), i);

    cout << "��������� ��������:" << endl
        << "1. �������(�� ID)" << endl
        << "0. �����" << endl;
}

int Interfaces::selectorViewAll() {
    switch (in.inputInt(0, 1, "�������� ��������:")) {
    case 1:
        return 100;
    case 0:
        return 0;
    default:
        throw Exception("�� ������������ ��������.", -1);
    }
}

void Interfaces::printAddNew() {        //  Code 300
    system("CLS");
    cout << title;
    cout << "���� ������������:" << endl
        << "1. ���, �������" << endl
        << "2. ������ �������" << endl
        << "3. ��������" << endl
        << "4. ������ � �������" << endl
        << "5. ������������" << endl
        << "0. �����" << endl;
}

void Interfaces::selectorAddNew() {
    switch (in.inputInt(0, 5, "�������� ��� ������������:")) {
    case 1:
        system("CLS");
        cout << "\t�������� ���/�������:" << endl;
        storage.addHouse();
        break;
    case 2:
        system("CLS");
        cout << "\t�������� �������:" << endl;
        storage.addEmpty();
        break;
    case 3:
        system("CLS");
        cout << "\t�������� ��������:" << endl;
        storage.addFlat();
        break;
    case 4:
        system("CLS");
        cout << "\t�������� �����/��������:" << endl;
        storage.addParking();
        break;
    case 5:
        system("CLS");
        cout << "\t�������� ������������ ���������:" << endl;
        storage.addCommercial();
        break;
    case 0:
        return;
    default:
        cout << "����� �������� ��� ������������." << endl
            << "����������� � ������� ���� ����� 2 �������...";
        Sleep(5000);
        return;
    }
    cout << "������ ��� ��������. ����������� � ������� ���� ����� 2 �������...";
    Sleep(2000);
}

// 210
void Interfaces::printViewItem(string className, unsigned int id, bool advancedMode) {
    if (advancedMode) {
        resentId = id;
        system("CLS");
    }

    MyVector<House>& houses = storage.listHouses;
    MyVector<Empty>& empties = storage.listEmpty;
    MyVector<Flat>& flats = storage.listFlat;
    MyVector<Parking>& parkings = storage.listParking;
    MyVector<Commercial>& commercials = storage.listCommercial;

    if (className == "house") {
        printListItem(houses, id);
    }
    else if (className == "empty") {
        printListItem(empties, id);
    }
    else if (className == "flat") {
        printListItem(flats, id);
    }
    else if (className == "parking") {
        printListItem(parkings, id);
    }
    else if (className == "commercial") {
        printListItem(commercials, id);
    }

    if (advancedMode) {
        cout << "��������� ��������:" << endl
            << "1. �������������\n"
            "2. �������\n"
            "0. �����\n";
    }
}

template<typename T>
void Interfaces::printListItem(const MyVector<T>& array, unsigned int id) {
    for (auto it = array.begin(); it != array.end(); ++it) {
        if (it->getId() == id) {
            it->printInfo();
            return;
        }
    }
}

int Interfaces::selectorViewItem() {
    switch (in.inputInt(0, 2, "�������� ��������:")) {
    case 1:
        return 111; //  edit
    case 2:
        return 112; //  hide
    case 0:
        return 100; //  (come back) to find by ID
    default:
        throw Exception("�� ������������ ��������.", -1);
    }
}

    void Interfaces::actionOnObject(unsigned int id, bool justHide) {
        for (auto it = storage.listHouses.begin(); it != storage.listHouses.end(); ++it) {
            if (it->getId() == id) {
                if (justHide)
                    hideObject(*it);
                else {
                    if (editObject(*it))
                        editHouse(*it);
                }
                storage.upload(*it);
                return;
            }
        }

        for (auto it = storage.listEmpty.begin(); it != storage.listEmpty.end(); ++it) {
            if (it->getId() == id) {
                if (justHide)
                    hideObject(*it);
                else {
                    if (editObject(*it))
                        editEmpty(*it);
                }
                storage.upload(*it);
                return;
            }
        }
        for (auto it = storage.listFlat.begin(); it != storage.listFlat.end(); ++it) {
            if (it->getId() == id) {
                if (justHide)
                    hideObject(*it);
                else {
                    if (editObject(*it))
                        editFlat(*it);
                }
                storage.upload(*it);
                return;
            }
        }
        for (auto it = storage.listParking.begin(); it != storage.listParking.end(); ++it) {
            if (it->getId() == id) {
                if (justHide)
                    hideObject(*it);
                else {
                    if (editObject(*it))
                        editParking(*it);
                }
                storage.upload(*it);
                return;
            }
        }
        for (auto it = storage.listCommercial.begin(); it != storage.listCommercial.end(); ++it) {
            if (it->getId() == id) {
                if (justHide)
                    hideObject(*it);
                else {
                    if (editObject(*it))
                        editCommercial(*it);
                }
                storage.upload(*it);
                return;
            }
        }
 
}

template<typename T>
bool Interfaces::editObject(T& object) {
    if (!object.getActuality()) {
        cout << "��������� ��������:\n"
            "1. ������ ������� ����������\n"
            "2. ������� ���������� � ���������������\n"
            "3. ������ �������������\n";
        int answer = in.inputInt(1, 3, "�������� ��������:");
        switch (answer) {
        case 1:
            object.setActuality(true);
            return false;
        case 2:
            object.setActuality(true);
        case 3:
            break;
        default:
            throw Exception("������� ������������ ��������.\n ��� ������ ������� ��������.", 1);
        }
    }
    system("CLS");
    bool flag = object.getActuality();
    object.setActuality(true);
    object.printInfo();
    object.setActuality(flag);

    editImmovable(object);
    return true;
}

template<typename T>
void Interfaces::editImmovable(T& object) {
    ObjectManager manager;
    try {
        //  Contacts
        object.setMobile(manager.requestPhone());
        object.setEmail(manager.requestEmail());
        //  for Immovable
        object.setCost(manager.requestCost());
        object.setSquare(manager.requestSqr());
        object.setAddress(manager.requestAddr());
    }
    catch (...) {}
}

template<typename T>
void Interfaces::editPiece(T& object) {
    ObjectManager manager;
    try {
        object.setPond(manager.requestPond());
        object.setPlants(manager.requestPlats());
        object.setCommunications(manager.requestCommun());
    }
    catch (...) {}
}

void Interfaces::editHouse(House& object) {
    editPiece(object);
    ObjectManager manager;
    try {
        object.setRooms(manager.requestRooms());
        object.setFloors(manager.requestFloor());
        object.setParking(manager.requestParking());
    }
    catch (...) {}
}

void Interfaces::editEmpty(Empty& object) {
    editPiece(object);
    ObjectManager manager;
    try {
        object.setFarming(manager.requestSuiFFarm());
        object.setConstruction(manager.requestSuiFCons());
    }
    catch (...) {}
}

void Interfaces::editFlat(Flat& object) {
    ObjectManager manager;
    try {
        object.setRooms(manager.requestRooms());
        object.setFloor(manager.requestFloor(true));
        object.isHaveBalcony(manager.requestHaveBalcony());
    }
    catch (...) {}
}

void Interfaces::editParking(Parking& object) {
    ObjectManager manager;
    try {
        object.setType(manager.requestType(true));
    }
    catch (...) {}
}

void Interfaces::editCommercial(Commercial& object) {
    ObjectManager manager;
    try {
        object.setType(manager.requestType());
    }
    catch (...) {}
}

template<typename T>
void Interfaces::hideObject(T& object) {
    object.setActuality(false);
}
