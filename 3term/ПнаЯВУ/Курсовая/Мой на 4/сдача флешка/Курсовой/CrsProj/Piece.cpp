#include "Piece.h"

Piece::Piece(unsigned int id, const string& phonePrimary, const string& email, float cost, double square,
    const string& address, bool actuality, bool pond, bool plants, bool communications)
    : Immovable(id, phonePrimary, email, cost, square, address, actuality) {
    setPond(pond);
    setPlants(plants);
    setCommunications(communications);
}

Piece::~Piece() {}

void Piece::setPond(bool available) {
    availablePond = available;
}

bool Piece::pond() {
    return availablePond;
}

void Piece::setPlants(bool available) {
    availablePlants = available;
}

bool Piece::plants() {
    return availablePlants;
}

void Piece::setCommunications(bool available) {
    availabilityOfCommunications = available;
}

bool Piece::communications() {
    return availabilityOfCommunications;
}
