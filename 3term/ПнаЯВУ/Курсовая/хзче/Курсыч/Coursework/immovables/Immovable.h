//
// Created by Egor on 11/27/2022.
//

#ifndef COURSEWORK_IMMOVABLE_H
#define COURSEWORK_IMMOVABLE_H

#include <locale>
#include <iostream>
#include <iomanip>
//#include "../services/ContactDetails.h"

using namespace std;

class Immovable {
    class ContactDetails {
        string phoneNumber;
        string email;
    public:
        ContactDetails(const string &phonePrimary = nullptr, const string &email = nullptr) {
            phoneNumber = phonePrimary;
            this->email = email;
        }

        string getContactDetails() {
            if (phoneNumber.empty() && email.empty())
                return "Нет контактной информации";
            string str = "  Для связи:";
            if (!phoneNumber.empty())
                str = str + "\nМобильный: " + phoneNumber;
            if (!email.empty())
                str = str + "\nE-mail: " + email;
            return str;
        }
    };

    unsigned int id;
//protected:
    bool isActual;
    float cost;
    double square;
    string address;
    ContactDetails *contact;

protected:
    string lineStr = "----------------------------\n";
    string boolToString(bool positive) {
        if (positive)
            return "Да";
        return "Нет";
    }

public:
    Immovable(unsigned int id, const string &phonePrimary = nullptr, const string &email = nullptr, float cost = -1,
              double square = -1,
              const std::string &address = nullptr, bool actuality = true) {
        this->id = id;  // Сделать исключение, если ID - не указан, то ОШИБКА.
        contact = new Immovable::ContactDetails(phonePrimary, email);
        setCost(cost);
        setSquare(square);
        setAddress(address);
        setActuality(true);
    }

    ~Immovable() = default;

    virtual void printInfo() = 0;

    //  contact
//    void setContact(const string &mobile = nullptr, const string &email = nullptr) {
//        contact = *new ContactDetails(mobile, email);
//    }

    std::string getContact() {
//        return contact.getContactDetails();
        return contact->getContactDetails();
    }

    //  address
    void setAddress(const std::string &value) {
        address = value;
    }

    std::string getAddress() {
        return address;
    }

    //  square
    void setSquare(double value) {
        square = value;
    }

    std::string printSquare() {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << square;
        return stream.str() + " m^2";
    }

    double getSquare() {
        return square;
    }

    //  cost
    void setCost(float value) {
        cost = value;
    }

    std::string printCost() {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << cost;
        return stream.str() + "$";
    }

    float getCost() {
        return cost;
    }

    // id
    bool cmpId(int id) {
        if (this->id == id)
            return true;
        return false;
    }

    int getId() {
        return id;
    }

    //  actuality

    void setActuality(bool isActual) {
        this->isActual = isActual;
    }

    bool getActuality() {
        return isActual;
    }
};


#endif //COURSEWORK_IMMOVABLE_H
