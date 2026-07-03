//
// Created by Egor on 12/4/2022.
//

#ifndef COURSEWORK_CONTACTDETAILS_H
#define COURSEWORK_CONTACTDETAILS_H

#include <string>
#include "Exception.h"

using namespace std;

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
            return "No contact information";
        string str = "To contact:";
        if (!phoneNumber.empty())
            str = str + "\nMobile: " + phoneNumber;
        if (!email.empty())
            str = str + "\nE-mail: " + email;
        return str;
    }
};

#endif //COURSEWORK_CONTACTDETAILS_H