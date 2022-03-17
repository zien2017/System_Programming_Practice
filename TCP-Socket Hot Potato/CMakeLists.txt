cmake_minimum_required(VERSION 3.10)
project(TCP_Socket_Hot_Potato C)

set(CMAKE_C_STANDARD 99)

add_executable(ringmaster ringmaster.c potato.h socketOP.h csapp.h csapp.c)

add_executable(player player.c potato.h socketOP.h csapp.h csapp.c)

