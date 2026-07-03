#include <iostream>
#include "factorys/headers/Motorcycle.h"

//              /   Bike   -->       Moped       \
//  Factory -->                                    --> Motorbike
//              \  Scooter --> Electric Scooter  /

int main() {
    Motorcycle moto("Audi AG", 21700, "Ducati", 275, 167, 148, 5.5);
    std::cout << moto;
    return 0;
}