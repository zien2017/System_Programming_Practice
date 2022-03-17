//
// Created by Brandon on 3/17/2022.
//
//
//#include <stdio.h>
//#include <stdlib.h>

#include "csapp.h"

// get address info from hostname
void getAddrInfo(char* hostName, struct addrinfo *listp)
{
    struct addrinfo *p, hints;
    char buf[MAXLINE];
    int rc, flags;
    /* Get a list of addrinfo records */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM; /* Connections only */
    if ((rc = getaddrinfo(hostName, NULL, &hints, &listp)) != 0) {
        fprintf(stdout, "Err: getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }
    /* Walk the list and display each IP address */
    flags = NI_NUMERICHOST; /* Display address instead of name */
    for (p = listp; p; p = p->ai_next) {
        Getnameinfo(p->ai_addr, p->ai_addrlen,
                    buf, MAXLINE, NULL, 0, flags);
        printf("\taddr is: %s\n", buf);
    }
    /* Clean up */
    Freeaddrinfo(listp);
}

// input checker
void input_parser (int argc, char** argv) {

    if (argc != 3) {
        printf("Arguments usage: player <machine_name> <port_num>\n");
        exit(1);
    }

    char* machine_name = argv[1];
    int port_num = atoi(argv[2]);
    if (port_num <= 0 || 65536 < port_num) {
        printf("ERR: port_num error: %d\n", port_num);
        exit(1);
    }

    printf("this is a player with\n");

    struct addrinfo * listp;
    getAddrInfo(machine_name, listp);


    printf("\tmachine_name is %s\n", machine_name);
    printf("\tport_num is %d\n", port_num);

}


/*
 * Arguments usage: player <machine_name> <port_num>
 * where machine_name is the machine name (e.g. dku-vcm-508.vm.duke.edu) where the ringmaster
 * process is running and port_num is the port number given to the ringmaster process
 */
int main(int argc, char** argv) {
    // argv[1] machine name of ringmaster
    // argv[2] port_num of ringmaster

    input_parser(argc, argv);

    return 0;
}
