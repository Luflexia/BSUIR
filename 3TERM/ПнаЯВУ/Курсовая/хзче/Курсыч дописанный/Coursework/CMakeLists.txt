cmake_minimum_required(VERSION 3.23)
project(Coursework)

set(CMAKE_CXX_STANDARD 14)

add_executable(Coursework main.cpp immovables/Immovable.h immovables/commercial/Commercial.h immovables/parking/Parking.h services/ContactDetails.h services/Exception.h services/Storage.cpp services/Storage.h interfaces/MainMenu.cpp interfaces/MainMenu.h services/Input.cpp services/Input.h immovables/piece/Piece.h immovables/piece/House.h immovables/piece/Empty.h immovables/flat/Flat.h interfaces/Interfaces.cpp interfaces/Interfaces.h)
