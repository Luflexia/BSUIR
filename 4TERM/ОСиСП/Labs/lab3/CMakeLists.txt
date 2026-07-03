cmake_minimum_required(VERSION 3.28)
project(OSiSPl3 C)

set(CMAKE_C_STANDARD 11)

add_executable(parent parent.c)
add_executable(child child.c)
