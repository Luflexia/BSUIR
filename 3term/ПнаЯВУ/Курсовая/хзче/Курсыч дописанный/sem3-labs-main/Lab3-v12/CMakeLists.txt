cmake_minimum_required(VERSION 3.23)
project(Lab3_v12)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab3_v12 main.cpp Factory.cpp Factory.h factorys/sources/Bicycle.cpp factorys/headers/Bicycle.h factorys/sources/Moped.cpp factorys/headers/Moped.h factorys/sources/Motorcycle.cpp factorys/headers/Motorcycle.h factorys/sources/ElectricScooter.cpp factorys/headers/ElectricScooter.h factorys/sources/Scooter.cpp factorys/headers/Scooter.h)
