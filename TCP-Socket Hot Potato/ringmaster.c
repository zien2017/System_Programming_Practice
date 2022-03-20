//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "potato.h"
#include "socket_server.h"

// input checker
void input_parser (int argc, char** argv, int* port_num, int* num_players, int* num_hops) {
    if (argc != 4) {
        printf("Arguments usage: ringmaster <port_num> <num_players> <num_hops>\n");
        exit(1);
    }

    * port_num = atoi(argv[1]);
    * num_players = atoi(argv[2]);
    * num_hops = atoi(argv[3]);

    if (*port_num <= 0 || 65536 < *port_num) {
        printf("ERR: port_num error: %d\n", * port_num);
        exit(1);
    }
    if (*num_players <= 0 || 300 < *num_players) {
        printf("ERR: num_players error: %d\n", * num_players);
        exit(1);
    }
    if (* num_hops <= 0 || 65536 < * num_hops) {
        printf("ERR: num_hops error: %d\n", * num_hops);
        exit(1);
    }

    return;
}

// malloc a new potato
struct _potato * set_potato (int num_hops) {
    struct _potato * potato = malloc(sizeof(struct _potato));
    memset(potato, 0, sizeof(struct _potato));
    return potato;
}


int server_new_connection (int listener, int fdmax, char* remoteIP, socklen_t * addrlen_p, struct sockaddr_storage * remoteaddr_p, fd_set * master_p) {
    *addrlen_p = sizeof (*remoteaddr_p);

    int newfd = accept(listener, // newly accept()ed socket descriptor
                       (struct sockaddr *)remoteaddr_p,
                       addrlen_p);
    // handle new connections
    if (newfd == -1) {
        perror("accept");
    } else {
        FD_SET(newfd, master_p); // add to master set
        if (newfd > fdmax) {    // keep track of the max
            fdmax = newfd;
        }
        printf("selectserver: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr_p->ss_family,
                         get_in_addr((struct sockaddr*)remoteaddr_p),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);
    }
    return fdmax;
}


void server_recv_data (int i, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p) {
    // handle data from a client
    if ((nbytes = recv(i, buf, sizeof_buf, 0)) <= 0) {
        // got error or connection closed by client
        if (nbytes == 0) {
            // connection closed
            printf("selectserver: socket %d hung up\n", i);
        } else {
            perror("recv");
        }
        close(i); // bye!
        FD_CLR(i, master_p); // remove from master set
    } else {
        // we got some data from a client
        for(int j = 0; j <= fdmax; j++) {
            // send to everyone!
            if (FD_ISSET(j, master_p)) {
                // except the listener and ourselves
                if (j != listener && j != i) {
                    if (send(j, buf, nbytes, 0) == -1) {
                        perror("send");
                    }
                }
            }
        }
    }
}

int server_main_loop (int listener) {
    /////////////////////////////////////////////////////////////

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
    } // END for(;;)--and you thought it would never end!

    return 0;
}

void server_close(int socket_fd) {
    close(socket_fd);
}




// Arguments usage: ringmaster <port_num> <num_players> <num_hops>
int main(int argc, char** argv) {
    int port_num, num_players, num_hops;
    input_parser (argc, argv, &port_num, &num_players, &num_hops);
    printf("this is a ringmaster with\n");
    printf("\tport_num is %d\n", port_num);
    printf("\tnum_players is %d\n", num_players);
    printf("\tnum_hops is %d\n", num_hops);

    struct _potato* potato = set_potato (num_hops);

    int listener_fd = server_setup (argv[1]);

    server_main_loop(listener_fd);

    server_close(listener_fd);

    return 0;
}


