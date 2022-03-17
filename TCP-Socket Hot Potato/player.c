//
// Created by Brandon on 3/17/2022.
//
//
//#include <stdio.h>
//#include <stdlib.h>
#include "socketOP.h"

/*
 * Arguments usage: player <machine_name> <port_num>
 * where machine_name is the machine name (e.g. dku-vcm-508.vm.duke.edu) where the ringmaster
 * process is running and port_num is the port number given to the ringmaster process
 */
int main(int argc, char** argv) {
    // argv[1] machine name of ringmaster
    // argv[2] port_num of ringmaster

    if (argc != 3) {
        printf("Arguments usage: player <machine_name> <port_num>\n");
        return -1;
    }


    char* machine_name = argv[1];
    int port_num = atoi(argv[2]);
    if (port_num < 0 || 65536 < port_num) {
        fprintf(stdout, "port_num error: %d\n", port_num);
        exit(1);
    }

    printf("this is a player with\n");

    printf("\tmachine_name is %s\n", machine_name);
    printf("\tport_num is %d\n", port_num);

    struct addrinfo * listp;
    getAddrInfo("localhost", listp);

    return 0;
}
