//
// Created by Brandon on 3/17/2022.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "socket_client.h"
#include "socket_select_server.h"

#define PORT "22222"

int sockfd_client_for_ringmaster = 0;
int sockfd_server_ring_right = 0;
int sockfd_client_ring_left = 0;



// input checker
void input_checker (int argc, char** argv) {

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

    printf("\tmachine_name is %s\n", machine_name);
    printf("\tport_num is %d\n", port_num);

}


void got_msg_from_ringmaster (int nbytes, char* buf) {

    buf[nbytes] = '\0';
    printf("client: received (length = %d) '%s'\n", nbytes, buf);


    memset(buf, 0, sizeof(buf));

}

void got_msg_from_one_end (int nbytes, char* buf) {

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
                         get_in_addr((struct sockaddr *) remoteaddr_p),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);
    }
    return 0;
}

void server_recv_data (int i, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p) {
    printf("ERR: No data should be received here!");
}

int player_main_loop () {

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

    // add the fds to the master set
    FD_SET(sockfd_client_for_ringmaster, &master);
    FD_SET(sockfd_client_ring_left, &master);
    FD_SET(sockfd_server_ring_right, &master);

    // keep track of the biggest file descriptor
    fdmax = 4;


    // main loop
    for(;;) {
        read_fds = master; // copy it

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            // we got one!!
            if ( ! FD_ISSET(i, &read_fds)) {
                continue;
            }
            if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                // got error or connection closed by client
                if (nbytes == 0) {
                    // connection closed
                    printf("selectserver: socket %d hung up\n", i);
                } else {
                    perror("recv");
                }
                close(i); // bye!
                FD_CLR(i, &master); // remove from master set
                continue;
            }
            if (i == sockfd_client_for_ringmaster) {
                got_msg_from_ringmaster(nbytes, buf);

            }
            if (i == sockfd_client_ring_left) {
                got_msg_from_one_end(nbytes, buf);

            }
            if (i == sockfd_server_ring_right) {
                got_msg_from_one_end(nbytes, buf);
            }
            else {


            } // END handle data from client

        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}


void register_to_ringmaster () {

//    char send_buf[100] = "\0";

    if (send(sockfd_client_for_ringmaster, PORT, 10, 0) == -1)
        perror("send");

    printf("my port is %s\n", PORT);
}

/*
 * Arguments usage: player <machine_name> <port_num>
 * where machine_name is the machine name (e.g. dku-vcm-508.vm.duke.edu) where the ringmaster
 * process is running and port_num is the port number given to the ringmaster process
 */
int main(int argc, char** argv) {
    // argv[1] machine name of ringmaster
    // argv[2] port_num of ringmaster

    input_checker(argc, argv);


    // set sockfd_server_ring_right
    int listener_fd = server_setup (PORT);


    // set client fd from ringmaster
    sockfd_client_for_ringmaster = client_setup (argv[1], argv[2]);

    // register to the ringmaster
    register_to_ringmaster ();

    // main loop
    player_main_loop();

//    ring_setup();

    // set sockfd_client_ring_left
//    sockfd_client_ring_left = client_setup (NULL, NULL);




    client_close (sockfd_client_for_ringmaster);


    return 0;
}
