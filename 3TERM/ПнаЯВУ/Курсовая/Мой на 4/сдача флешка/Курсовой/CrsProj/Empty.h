#ifndef COURSEWORK_EMPTY_H
#define COURSEWORK_EMPTY_H

#include "Piece.h"

class Empty : public Piece {
    bool suitableForConstruction;
    bool suitableForFarming;
public:
    Empty(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality, bool pond, bool plants, bool communications,
        bool suitableForConstruction = true, bool suitableForFarming = true);

    ~Empty();

    void printInfo() override;

    friend ostream& operator<<(ostream& out, Empty& myClass);

    void setConstruction(bool suitable);

    bool suitableConstruction();

    void setFarming(bool suitable);

    bool suitableFarming();
};

#endif //COURSEWORK_EMPTY_H
