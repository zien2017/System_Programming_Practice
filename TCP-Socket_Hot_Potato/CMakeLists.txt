cmake_minimum_required(VERSION 2.4)
project(TCP_Socket_Hot_Potato C)


set(CMAKE_C_STANDARD 99)


add_executable(ringmaster ringmaster.c potato.h socket_select_server.h message_wrapper.h)

add_executable(player player.c potato.h socket_client.h message_wrapper.h)

