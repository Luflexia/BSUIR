cmake_minimum_required(VERSION 3.22.3)
project(Coursework)

set(CMAKE_CXX_STANDARD 14)

add_executable(Coursework main.cpp immovables/Immovable.h immovables/commercial/Commercial.h immovables/parking/Parking.h services/Exception.h services/Storage.cpp services/Storage.h services/Input.cpp services/Input.h immovables/piece/Piece.h immovables/piece/House.h immovables/piece/Empty.h immovables/flat/Flat.h interfaces/Interfaces.cpp interfaces/Interfaces.h services/ObjectManager.h)
