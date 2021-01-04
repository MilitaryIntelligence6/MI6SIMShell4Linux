cmake_minimum_required(VERSION 3.16)
project(Linux C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Linux
        include/sims.h
        src/fun.c
        src/main.c)
