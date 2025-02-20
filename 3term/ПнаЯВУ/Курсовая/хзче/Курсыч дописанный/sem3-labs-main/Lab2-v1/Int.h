#ifndef LAB2_V1_INT_H
#define LAB2_V1_INT_H

class Int {
private :
    int value;
public :
    Int(int value = 0) {
        this->value = value;
    }

    void setValue(int value) { this->value = value; }

    int getValue() { return value; }

    /// Methods...
    Int& operator=(Int x);

    Int& operator()(int x);

    std::string operator[](int);

    Int operator++();       //  Prefix
    Int operator++(int);    //  Postfix

    Int operator+(Int b);

    bool operator>(Int b);

    ///     Friendly methods
    friend Int operator--(Int &x);      //  Prefix
    friend Int operator--(Int &x, int); //  Postfix

    friend Int operator-(const Int& a, const Int& b);
    friend Int operator-(const Int& a, int b);
    friend Int operator-(int a, const Int& b);

    friend bool operator<(const Int& a, const Int& b);
    friend bool operator<(int a, const Int& b);
    friend bool operator<(const Int& a, int b);

    friend std::ostream& operator<< (std::ostream &out, const Int &value);
};

#endif //LAB2_V1_INT_H