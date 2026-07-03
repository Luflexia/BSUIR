#include <iostream>
#include "Int.h"

///     Methods
//  =
Int &Int::operator=(Int x) {
    (*this).setValue(x.getValue());
    return *this;
}

//  ()
Int &Int::operator()(int x = 0) {
    value = x;
    return *this;
}

//  []
std::string Int::operator[](int) {
    return "Unable to index class Int";
}

//  ++
Int Int::operator++() { //  Prefix
    return Int(++value);
}

Int Int::operator++(int) { //  Postfix
    return Int(value++);
}

//  +
Int Int::operator+(Int b) {
    return Int(value + b.getValue());
}

//  >
bool Int::operator>(Int b) {
    return bool(value > b.getValue());
}

///     Friendly methods
//  --
Int operator--(Int &x) { //  Prefix
    return Int(--x.value);
}

Int operator--(Int &x, int) { //  Postfix
    return Int(x.value--);
}

//  -
Int operator-(const Int &a, const Int &b) {
    return Int(a.value - b.value);
}

Int operator-(int a, const Int &b) {
    return Int(a - b.value);
}

Int operator-(const Int &a, int b) {
    return Int(a.value - b);
}

//  <
bool operator<(const Int &a, const Int &b) {
    return bool(a.value < b.value);
}

bool operator<(int a, const Int &b) {
    return bool(a < b.value);
}

bool operator<(const Int &a, int b) {
    return bool(a.value < b);
}

//  <<
std::ostream &operator<<(std::ostream &out, const Int &x) {
    out << x.value;
    return out;
}