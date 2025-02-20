#include "Immovable.h"

Immovable::ContactDetails::ContactDetails(const string& phonePrimary, const string& email) {
    setMobile(phonePrimary);
    setEmail(email);
}

void Immovable::ContactDetails::setMobile(const string& phone) {
    phoneNumber = phone;
}

string Immovable::ContactDetails::getMobile() {
    return phoneNumber;
}

void Immovable::ContactDetails::setEmail(const string& email) {
    this->email = email;
}

string Immovable::ContactDetails::getEmail() {
    return email;
}

string Immovable::ContactDetails::getContactDetails() {
    if (phoneNumber.empty() && email.empty())
        return "Нет контактной информации";
    string str = "Для связи:";
    if (!phoneNumber.empty())
        str = str + "\nМобильный: " + phoneNumber;
    if (!email.empty())
        str = str + "\nE-mail: " + email;
    return str;
}

Immovable::Immovable(unsigned int id, const string& phone, const string& email, float cost,
    double square, const string& address, bool actuality) {
    this->id = id;
    contact = new Immovable::ContactDetails(phone, email);
    setCost(cost);
    setSquare(square);
    setAddress(address);
    setActuality(actuality);
}

string Immovable::boolToString(bool positive) {
    if (positive)
        return "Да";
    return "Нет";
}

//  contact
string Immovable::getContact() {
    return contact->getContactDetails();
}

void Immovable::setMobile(const string& newMobile) {
    contact->setMobile(newMobile);
}

string Immovable::getMobile() {
    return contact->getMobile();
}

void Immovable::setEmail(const string& newEmail) {
    contact->setEmail(newEmail);
}

string Immovable::getEmail() {
    return contact->getEmail();
}

//  address
void Immovable::setAddress(const string& value) {
    address = value;
}

string Immovable::getAddress() {
    return address;
}

//  square
double Immovable::setSquare(double value) {
    return square = value;
}

string Immovable::printSquare() {
    stringstream stream;
    stream << std::fixed << std::setprecision(1) << square;
    return stream.str() + " м^2";
}

double Immovable::getSquare() {
    return square;
}

//  cost
void Immovable::setCost(float value) {
    cost = value;
}

string Immovable::printCost() {
    stringstream stream;
    stream << fixed << setprecision(2) << cost;
    return stream.str() + "$";
}

float Immovable::getCost() {
    return cost;
}

// id
int Immovable::getId() {
    return id;
}

//  actuality
void Immovable::setActuality(bool isActual) {
    this->isActual = isActual;
}

bool Immovable::getActuality() {
    return isActual;
}
