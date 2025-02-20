#ifndef COURSEWORK_OBJECTMANAGER_H
#define COURSEWORK_OBJECTMANAGER_H

#include <string>
#include <fstream>
#include <windows.h>
#include "Exception.h"
#include "Input.h"

using namespace std;

class ObjectManager {
    Input in;
public:
    string requestPhone() {
        return in.inputMobile();
    }

    string requestEmail() {
        return in.inputEmail();
    }

    float requestCost() {
        return in.inputFloat(50, 2000000000, "������� ����: ");
    }

    double requestSqr() {
        return in.inputDouble(2, 100000000, "������� ����� �������: ");
    }

    string requestAddr() {
        return in.inputString("������� �����:");
    }

    bool requestPond() {
        return in.inputBool("���� ������?");
    }

    bool requestPlats() {
        return in.inputBool("���� �������/����������?");
    }

    bool requestCommun() {
        return in.inputBool("��������� �� ������������?");
    }

    int requestFloor(bool isFlat = false) {
        if (isFlat)
            return in.inputInt(1, 100, "������� �� ����� �����?");
        return in.inputInt(1, 5, "������� ������?");
    }

    int requestRooms() {
        return in.inputInt(1, 100, "������� ������?");
    }

    int requestParking() {
        return in.inputInt(1, 100, "������� ����������� ����?");
    }

    bool requestSuiFCons() {
        return in.inputBool("�������� �� ��� �������������?");
    }

    bool requestSuiFFarm() {
        return in.inputBool("�������� �� ��� ����������?");
    }

    bool requestHaveBalcony() {
        return in.inputBool("���� �� ������?");
    }

    int requestType(bool isParking = false) {
        if (isParking) {
            string msg = "1. ������ �����\n"
                "2. ����\n"
                "3. �����\n"
                "4. ������\n"
                "�������� ���:";
            return in.inputInt(1, 4, msg);
        }
        string msg = "1. ����\n"
            "2. �������, �������� ���������\n"
            "3. �����\n"
            "4. ������\n"
            "�������� ���:";
        return in.inputInt(1, 4, msg);
    }
};

#endif //COURSEWORK_OBJECTMANAGER_H
