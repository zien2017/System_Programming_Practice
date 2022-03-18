cmake_minimum_required(VERSION 3.10)
project(TCP_Socket_Hot_Potato C)

set(CMAKE_C_STANDARD 99)

add_executable(ringmaster ringmaster.c potato.h)

add_executable(player player.c potato.h)

add_executable(server server.c)
add_executable(client client.c)

