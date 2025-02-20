//
// Created by Egor on 12/6/2022.
//

#ifndef COURSEWORK_PIECE_H
#define COURSEWORK_PIECE_H

#include "../Immovable.h"

class Piece : public Immovable {
    bool availablePond;
    bool availablePlants;
    bool availabilityOfCommunications;

public:
    Piece(unsigned int id, const string &phonePrimary, const string &email, float cost, double square,
          const string &address, bool actuality, bool pond = false, bool plants = true, bool communications = false)
            : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
        setPond(pond);
        setPlants(plants);
        setCommunications(communications);
    }

    ~Piece() override = default;

    void setPond(bool available) {
        availablePond = available;
    }

    bool pond() {
        return availablePond;
    }

    void setPlants(bool available) {
        availablePlants = available;
    }

    bool plants() {
        return availablePlants;
    }

    void setCommunications(bool available) {
        availabilityOfCommunications = available;
    }

    bool communications() {
        return availabilityOfCommunications;
    }
};

#endif //COURSEWORK_PIECE_H