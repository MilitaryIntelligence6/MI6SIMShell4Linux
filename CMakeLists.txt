cmake_minimum_required(VERSION 3.16)
project(MI6SimShell4Linux C)

set(CMAKE_C_STANDARD 11)

add_executable(MI6SimShell4Linux
        src/main.c
        include/sims.h
        src/sims.c
        )
