#include <cstdlib>
#include "Interfaces.h"
#include <iostream>
#include <fstream>
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
            removeObject(resentId);
            interfaceCode = 0;
            cout << "\n������������ ���� �������." << endl
                << "����������� � ������� ���� ����� 2 �������...";
            Sleep(2000);
            break;
        case 113:  // ������ �������
            //hideObject(resentId);
            interfaceCode = 0;
            cout << "\n������������ ���� ������." << endl
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
        "2. �������� ������������\n"
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
    cout << "������������ ��������� ID: " << storage.getFreeId() - 1 << endl;
    cout << "������� ID('-1' - ���-�� ��������� �����):";
}

int Interfaces::selectorFindByID() {
    int id = in.inputInt(-1, storage.getFreeId() - 1);
    if (id == -1)
        return 0;
    printViewItem(storage.identifyObject(id), id, true);
    return 110;
}

void Interfaces::printViewAll() {
    system("CLS");
    cout << title;
    for (unsigned int i = 0; i < storage.getFreeId(); ++i)
        printViewItem(storage.identifyObject(i), i);

    cout << "��������� ��������:" << endl
        << "1. ������� �� ID" << endl
        << "2. ���������� �� ID" << endl
        << "3. ���������� �� ����" << endl
        << "0. �����" << endl;
}

int Interfaces::selectorViewAll() {
    switch (in.inputInt(0, 1, "�������� ��������:")) {
    case 1:
        return 100;
    case 2:
        return 100;
    case 3:
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
        Sleep(2000);
        return;
    }
    cout << "������ ��� ��������. ����������� � ������� ���� ����� 2 �������...";
    Sleep(2000);

    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

template<typename T>
void Interfaces::printListItem(vector<T> array, unsigned int id) {
    for (int i = 0; i < array.size(); ++i)
        if (array[i].getId() == id) {
            array[i].printInfo();
            return;
        }
}

//  Code 210
void Interfaces::printViewItem(string className, unsigned int id, bool advancedMode) {
    if (advancedMode) {
        resentId = id;
        system("CLS");
    }

    if (className == "house") {
        printListItem(storage.vectorHouses, id);
    }
    else if (className == "empty") {
        printListItem(storage.vectorEmpty, id);
    }
    else if (className == "flat") {
        printListItem(storage.vectorFlat, id);
    }
    else if (className == "parking") {
        printListItem(storage.vectorParking, id);
    }
    else if (className == "commercial") {
        printListItem(storage.vectorCommercial, id);
    }

    if (advancedMode) {
        cout << "��������� ��������:" << endl
            << "1. �������������\n"
            << "2. �������\n"
            << "3. ������\n" 
            << "0. �����\n";
    }
}

int Interfaces::selectorViewItem() {
    switch (in.inputInt(0, 3, "�������� ��������:")) {
    case 1:
        return 111; //  edit
    case 2:
        return 112; //  �������
    case 3:
        return 113; //  hide
    case 0:
        return 100; //  (come back) to find by ID
    default:
        throw Exception("�� ������������ ��������.", -1);
    }
}

template<typename T>
void Interfaces::removeObject(T& object) {
    if constexpr (std::is_same<T, House>::value) {
        auto it = remove(storage.vectorHouses.begin(), storage.vectorHouses.end(), object);
        storage.vectorHouses.erase(it, storage.vectorHouses.end());
    }
    else if constexpr (std::is_same<T, Empty>::value) {
        auto it = std::remove(storage.vectorEmpty.begin(), storage.vectorEmpty.end(), object);
        storage.vectorEmpty.erase(it, storage.vectorEmpty.end());
    }
    else if constexpr (std::is_same<T, Flat>::value) {
        auto it = std::remove(storage.vectorFlat.begin(), storage.vectorFlat.end(), object);
        storage.vectorFlat.erase(it, storage.vectorFlat.end());
    }
    else if constexpr (std::is_same<T, Parking>::value) {
        auto it = std::remove(storage.vectorParking.begin(), storage.vectorParking.end(), object);
        storage.vectorParking.erase(it, storage.vectorParking.end());
    }
    else if constexpr (std::is_same<T, Commercial>::value) {
        auto it = std::remove(storage.vectorCommercial.begin(), storage.vectorCommercial.end(), object);
        storage.vectorCommercial.erase(it, storage.vectorCommercial.end());
    }

    storage.delFile(resentId);
}
void Interfaces::actionOnObject(unsigned int id, bool justHide) {
    for (int i = 0; i < storage.vectorHouses.size(); ++i) {
        if (storage.vectorHouses[i].getId() == id) {
            if (justHide) {
                //hideObject(storage.listHouses[i]);
            }
            else {
                if (editObject(storage.vectorHouses[i])) {
                    editHouse(storage.vectorHouses[i]);
                }
            }
            storage.upload(storage.vectorHouses[i]);
            return;
        }
    }

    for (int i = 0; i < storage.vectorEmpty.size(); ++i) {
        if (storage.vectorEmpty[i].getId() == id) {
            if (justHide) {
                //hideObject(storage.listEmpty[i]);
            }
            else {
                if (editObject(storage.vectorEmpty[i])) {
                    editEmpty(storage.vectorEmpty[i]);
                }
            }
            storage.upload(storage.vectorEmpty[i]);
            return;
        }
    }

    for (int i = 0; i < storage.vectorFlat.size(); ++i) {
        if (storage.vectorFlat[i].getId() == id) {
            if (justHide) {
                //hideObject(storage.listFlat[i]);
            }
            else {
                if (editObject(storage.vectorFlat[i])) {
                    editFlat(storage.vectorFlat[i]);
                }
            }
            storage.upload(storage.vectorFlat[i]);
            return;
        }
    }

    for (int i = 0; i < storage.vectorParking.size(); ++i) {
        if (storage.vectorParking[i].getId() == id) {
            if (justHide) {
                //hideObject(storage.listParking[i]);
            }
            else {
                if (editObject(storage.vectorParking[i])) {
                    editParking(storage.vectorParking[i]);
                }
            }
            storage.upload(storage.vectorParking[i]);
            return;
        }
    }

    for (int i = 0; i < storage.vectorCommercial.size(); ++i) {
        if (storage.vectorCommercial[i].getId() == id) {
            if (justHide) {
                //hideObject(storage.listCommercial[i]);
            }
            else {
                if (editObject(storage.vectorCommercial[i])) {
                    editCommercial(storage.vectorCommercial[i]);
                }
            }
            storage.upload(storage.vectorCommercial[i]);
            return;
        }
    }
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
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
            break;  // ����������� break
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
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
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
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
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
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

void Interfaces::editEmpty(Empty& object) {
    editPiece(object);
    ObjectManager manager;
    try {
        object.setFarming(manager.requestSuiFFarm());
        object.setConstruction(manager.requestSuiFCons());
    }
    catch (...) {}
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

void Interfaces::editFlat(Flat& object) {
    ObjectManager manager;
    try {
        object.setRooms(manager.requestRooms());
        object.setFloor(manager.requestFloor(true));
        object.isHaveBalcony(manager.requestHaveBalcony());
    }
    catch (...) {}
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

void Interfaces::editParking(Parking& object) {
    ObjectManager manager;
    try {
        object.setType(manager.requestType(true));
    }
    catch (...) {}
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

void Interfaces::editCommercial(Commercial& object) {
    ObjectManager manager;
    try {
        object.setType(manager.requestType());
    }
    catch (...) {}
    try {
        storage.saveAppFile();
    }
    catch (Exception exception) {
        exception.what();
    }
}

//template<typename T>
//void Interfaces::hideObject(T& object) {
//    object.setActuality(false);
//}