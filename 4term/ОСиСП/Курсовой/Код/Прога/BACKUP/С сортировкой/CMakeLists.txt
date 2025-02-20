cmake_minimum_required(VERSION 3.28)
project(untitled3 C)


set(CMAKE_C_STANDARD 11)

add_executable(untitled3 main.c
        display.c
        read.c
        read.h)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
