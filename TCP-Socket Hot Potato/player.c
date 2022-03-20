//
// Created by Brandon on 3/17/2022.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "potato.h"
#include "socket_client.h"
#include "socket_select_server.h"
#include <pthread.h>

#define PORT "23335"


fd_set master, read_fds;


struct playerInfo {
    char player_addr[INET6_ADDRSTRLEN];
    int player_port;
    int player_fd;
    struct playerInfo * next;
} ;

int sockfd_client_for_ringmaster = -1;
int sockfd_server_RHS = -1;
int sockfd_client_LFS = -1;


void refresh_fd_set () {
//    FD_ZERO(&master);    // clear the master and temp sets
//    FD_ZERO(&read_fds);

    // add the fds to the master set
    printf("refreshing fd set: ");
    printf(" %d", sockfd_client_for_ringmaster);
    printf(" %d", sockfd_client_LFS);
    printf(" %d\n", sockfd_server_RHS);
    if (sockfd_client_for_ringmaster != -1)
        FD_SET(sockfd_client_for_ringmaster, &master);
    if (sockfd_client_LFS != -1)
        FD_SET(sockfd_client_LFS, &master);
    if (sockfd_server_RHS != -1)
        FD_SET(sockfd_server_RHS, &master);
}


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

    if (nbytes == sizeof(struct playerInfo)) {
        // assigned to set up a ring
        struct playerInfo* p_info = (struct playerInfo*) buf;

        char port_str[6];
        sprintf(port_str, "%d", p_info->player_port);

        printf("received player info from master'\n");
        printf("\taddr %s\n", p_info->player_addr);
        printf("\tport %s\n", port_str);

        // set sockfd_client_LFS
        sockfd_client_LFS = client_setup (p_info->player_addr, port_str);
        FD_SET(sockfd_client_LFS, &master);

        if (send(sockfd_client_LFS, "hi! ", 10, 0) == -1)
            perror("player send msg : ");

        return;
    }
    if (nbytes == sizeof(potato)) {
        // got a potato
        struct _potato * po = (struct potato*) buf;


        printf("received a potato from ringmaster'\n");
        printf("\tremaining_counter %d\n", po->remaining_counter);

        if (send(sockfd_server_RHS, "hi! i have a potato ", 20, 0) == -1)
            perror("player send msg : ");

        return;
    }


    printf  ("player: received (length = %d) '%s'\n", nbytes, buf);

}

void got_msg_from_one_end (int nbytes, char* buf) {
    printf  ("player: received (length = %d) '%s'\n", nbytes, buf);

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
//        if (newfd > fdmax) {    // keep track of the max
//            fdmax = newfd;
//        }
        printf("player: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr_p->ss_family,
                         get_in_addr((struct sockaddr *) remoteaddr_p),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);
    }
    sockfd_server_RHS = newfd;

    FD_SET(sockfd_server_RHS, &master);
    return -1;
}

void server_recv_data (int i, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p) {
    printf("ERR: No data should be received here!\n");
}

int player_main_loop () {


    char buf[sizeof potato];    // buffer for client data
    int nbytes;

//    fd_set master;    // master file descriptor list
//    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    refresh_fd_set ();


    // keep track of the biggest file descriptor

    fdmax = 10;

    // main loop
    for(;;) {

        read_fds = master; // copy it

        memset(buf, 0, sizeof(*buf));

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("ERR: player: ");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(int i = 0; i <= fdmax; i++) {
            // we got one!!
            if ( ! FD_ISSET(i, &read_fds)) {
                continue;
            }
            if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                // got error or connection closed by client
                if (nbytes == 0) {
                    // connection closed
                    printf("player: socket %d hung up\n", i);
                } else {
                    perror("recv");
                }
                close(i); // bye!
                FD_CLR(i, &master); // remove from master set
                continue;
            }
            if (i == sockfd_client_for_ringmaster) {
                got_msg_from_ringmaster(nbytes, buf);
                continue;
            }
            if (i == sockfd_client_LFS) {
                got_msg_from_one_end(nbytes, buf);
                continue;
            }
            if (i == sockfd_server_RHS) {
                got_msg_from_one_end(nbytes, buf);
                continue;
            }

            printf("ERR: No data should be received here!\n");
            printf("ERR: Got msg from un-know fd: %d!\n", i);

            // END handle data from client

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

void* server_main_loop_helper (void* listener) {
    server_main_loop(*(int*) listener);
    pthread_exit(NULL);
}

/*
 * Arguments usage: player <machine_name> <port_num>
 * where machine_name is the machine name (e.g. dku-vcm-508.vm.duke.edu) where the ringmaster
 * process is running and port_num is the port number given to the ringmaster process
 */
int main(int argc, char** argv) {
    // argv[1] machine name of ringmaster
    // argv[2] port_num of ringmaster
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    input_checker(argc, argv);


    // set sockfd_server_RHS
    int listener_fd = server_setup (PORT);

    pthread_t t;
    pthread_create(&t, NULL, server_main_loop_helper, (void*)& listener_fd);


    // set client fd from ringmaster
    sockfd_client_for_ringmaster = client_setup (argv[1], argv[2]);

    // register to the ringmaster
    register_to_ringmaster ();

    // main loop
    player_main_loop();

//    pthread_join(t, NULL);

    client_close (sockfd_client_for_ringmaster);


    return 0;
}
