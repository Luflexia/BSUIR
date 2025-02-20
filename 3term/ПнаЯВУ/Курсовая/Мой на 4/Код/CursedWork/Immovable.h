#ifndef COURSEWORK_IMMOVABLE_H
#define COURSEWORK_IMMOVABLE_H
#include "Input.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Immovable {
    class ContactDetails {
        string phoneNumber;
        string email;
    public:
        ContactDetails(const string& phonePrimary = nullptr, const string& email = nullptr) {
            setMobile(phonePrimary);
            setEmail(email);
        }

        void setMobile(const string& phone) {
            phoneNumber = phone;
        }

        string getMobile() {
            return phoneNumber;
        }

        void setEmail(const string& email) {
            this->email = email;
        }

        string getEmail() {
            return email;
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
    bool isActual;
    float cost;
    double square;
    string address;
    ContactDetails* contact;

protected:
    string lineStr = "----------------------------\n\n";

    string boolToString(bool positive) {
        if (positive)
            return "Да";
        return "Нет";
    }

public:
    Immovable(unsigned int id, const string& phone = nullptr, const string& email = nullptr, float cost = -1,
        double square = -1, const string& address = nullptr, bool actuality = true) {
        this->id = id;
        contact = new Immovable::ContactDetails(phone, email);
        setCost(cost);
        setSquare(square);
        setAddress(address);
        setActuality(actuality);
    }

    virtual void printInfo() = 0;

    //  contact
    string getContact() {
        return contact->getContactDetails();
    }

    void setMobile(const string& newMobile) {
        contact->setMobile(newMobile);
    }

    string getMobile() {
        return contact->getMobile();
    }

    void setEmail(const string& newEmail) {
        contact->setEmail(newEmail);
    }

    string getEmail() {
        return contact->getEmail();
    }

    //  address
    void setAddress(const string& value) {
        address = value;
    }

    string getAddress() {
        return address;
    }

    //  square
    void setSquare(double value) {
        square = value;
    }

    string printSquare() {
        stringstream stream;
        stream << fixed << setprecision(1) << square;
        return stream.str() + " м^2";
    }
    //string printSquare() {
    //    return to_string(static_cast<int>(square)) + " м^2";
    //}

    double getSquare() {
        return square;
    }

    //  cost
    void setCost(float value) {
        cost = value;
    }

    string printCost() {
        stringstream stream;
        stream << fixed << setprecision(2) << cost;
        return stream.str() + "$";
    }
 /*   string printCost() {
        return toStringWithPrecision(cost, 2) + "$";
    }*/

    float getCost() {
        return cost;
    }

    // id
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
