cmake_minimum_required(VERSION 3.16.3)
project(testCases C)

set(CMAKE_C_STANDARD 11)

add_executable(testCases
        equal.c
        my_malloc.h
        my_malloc.c)
