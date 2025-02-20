#ifndef COURSEWORK_PIECE_H
#define COURSEWORK_PIECE_H

#include "Immovable.h"

class Piece : public Immovable {
    bool availablePond;
    bool availablePlants;
    bool availabilityOfCommunications;

public:
    Piece(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
        const string& address, bool actuality, bool pond = false, bool plants = true, bool communications = false);

    ~Piece();

    void setPond(bool available);

    bool pond();

    void setPlants(bool available);

    bool plants();

    void setCommunications(bool available);

    bool communications();
};

#endif //COURSEWORK_PIECE_H
