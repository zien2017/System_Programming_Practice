cmake_minimum_required(VERSION 3.10)
project(TCP_Socket_Hot_Potato C)

set(CMAKE_C_STANDARD 99)

add_executable(TCP_Socket_Hot_Potato main.c ringmaster.c player.c potato.h)
