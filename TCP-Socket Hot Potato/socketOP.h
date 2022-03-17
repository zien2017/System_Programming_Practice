//
// Created by Brandon on 3/17/2022.
//

#ifndef TCP_SOCKET_HOT_POTATO_SOCKETOP_H
#define TCP_SOCKET_HOT_POTATO_SOCKETOP_H



//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <errno.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
#include "csapp.h"


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
        fprintf(stdout, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }
    /* Walk the list and display each IP address */
    flags = NI_NUMERICHOST; /* Display address instead of name */
    for (p = listp; p; p = p->ai_next) {
        Getnameinfo(p->ai_addr, p->ai_addrlen,
                    buf, MAXLINE, NULL, 0, flags);
        printf("%s\n", buf);
    }
    /* Clean up */
    Freeaddrinfo(listp);
}


#endif //TCP_SOCKET_HOT_POTATO_SOCKETOP_H
