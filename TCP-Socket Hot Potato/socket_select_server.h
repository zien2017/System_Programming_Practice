/*
** selectserver.c -- a cheezy multiperson chat server
*/

#ifndef TCP_SOCKET_HOT_POTATO_SOCKET_SELECT_SERVER_H
#define TCP_SOCKET_HOT_POTATO_SOCKET_SELECT_SERVER_H


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

int server_new_connection (int listener, int fdmax, char* remoteIP, socklen_t * addrlen_p, struct sockaddr_storage * remoteaddr_p, fd_set * master_p) ;

void server_recv_data (int fd, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p);

int server_main_loop (int listener) {

    struct sockaddr_storage remoteaddr; // client address

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    socklen_t addrlen;

    int i;
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one


    // main loop
    for(;;) {
        read_fds = master; // copy it

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    fdmax = server_new_connection (listener, fdmax, remoteIP, &addrlen, &remoteaddr, &master) ;
                } else {
                    server_recv_data (i, nbytes, fdmax, listener, buf, sizeof buf, &master) ;
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors

        if (fdmax <= 0) return 0;
    } // END for(;;)--and you thought it would never end!

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




#endif //TCP_SOCKET_HOT_POTATO_SOCKET_SELECT_SERVER_H