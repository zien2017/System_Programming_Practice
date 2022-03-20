/*
** selectserver.c -- a cheezy multiperson chat server
*/

#ifndef TCP_SOCKET_HOT_POTATO_SOCKET_SERVER_H
#define TCP_SOCKET_HOT_POTATO_SOCKET_SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//#define PORT "9034"   // port we're listening on

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int server_setup(char* port) {

    int listener;     // listening socket descriptor

    int yes = 1;        // for setsockopt() SO_REUSEADDR, below
    int rv;

	struct addrinfo hints, *ai, *p;


	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0) {
		fprintf(stderr, "select_server: %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p = ai; p != NULL; p = p->ai_next) {
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) {
			continue;
		}

		// lose the pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}

		break;
	}

	// if we got here, it means we didn't get bound
	if (p == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai); // all done with this


    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    return listener;

}




#endif //TCP_SOCKET_HOT_POTATO_SOCKET_SERVER_H