cmake_minimum_required(VERSION 3.0)
project(l71)

set(SOURCES
        main.c
        list.c
        ring.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
