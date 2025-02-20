cmake_minimum_required(VERSION 3.28)
project(MyCourseWork C)


set(CMAKE_C_STANDARD 11)

add_executable(MyCourseWork main.c
        display.c
        read.c
        read.h
        control.c
        control.h
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(MyCourseWork ${CURSES_LIBRARIES})
