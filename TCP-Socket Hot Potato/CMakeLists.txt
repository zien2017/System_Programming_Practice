cmake_minimum_required(VERSION 3.10)
project(TCP_Socket_Hot_Potato C)

set(CMAKE_C_STANDARD 99)

add_executable(ringmaster ringmaster.c potato.h socket_select_server.h)

add_executable(player player.c potato.h socket_client.h)

add_executable(socket_simple_server socket_simple_server.c)


