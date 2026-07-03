cmake_minimum_required(VERSION 3.27)
project(OSiSPl2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wno-unused-parameter -Wno-unused-variable -pedantic")


add_executable(parent parent.c)
add_executable(child child.c)

