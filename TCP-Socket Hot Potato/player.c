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

#define PORT "25299"


fd_set master, read_fds;
int fdmax = 10;

struct playerInfo {
    char player_addr[INET6_ADDRSTRLEN];
    int player_port;
    int player_fd;
    struct playerInfo * next;
} ;

int fd_ringmaster = -1;
int fd_client_LHS = -1;
int fd_server_RHS = -1;


void refresh_fd_set () {

    printf("refreshing fd: %d %d %d\n", fd_ringmaster, fd_client_LHS, fd_server_RHS);

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);


    // add the fds to the master set
    fdmax = fdmax > fd_ringmaster ? fdmax : fd_ringmaster;
    fdmax = fdmax > fd_client_LHS ? fdmax : fd_client_LHS;
    fdmax = fdmax > fd_server_RHS ? fdmax : fd_server_RHS;

    if (fd_ringmaster != -1)
        FD_SET(fd_ringmaster, &master);
    if (fd_client_LHS != -1)
        FD_SET(fd_client_LHS, &master);
    if (fd_server_RHS != -1)
        FD_SET(fd_server_RHS, &master);
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


void throw_potato(char* buf ){
    // received a potato
    struct _potato * p = (struct _potato *) buf;
    if (p->remaining_counter == 0) {
        // ending give back to ringmaster
        if (send(fd_ringmaster, p, sizeof (struct _potato), 0) == -1)
            perror("player send potato : ");
    } else {
        -- p->remaining_counter;
        // next hop give back to ringmaster

        printf("received a potato (%d)\n", p->remaining_counter);

        if (send(fd_server_RHS, p, sizeof (struct _potato), 0) == -1)
            perror("player send potato : ");
    }
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

        // set fd_client_LHS
        fd_client_LHS = client_setup (p_info->player_addr, port_str);
        refresh_fd_set();


        if (fd_client_LHS != -1 )
            if (send(fd_client_LHS, "hi from right", 10, 0) == -1)
                perror("player send msg to left ");
        if (fd_server_RHS != -1 )
            if (send(fd_server_RHS, "hi from left", 10, 0) == -1)
                perror("player send msg to right ");



        return;
    }
    if (nbytes == sizeof(potato)) {
        // got a potato
        throw_potato(buf);
        return;
    }


    printf  ("player: received (length = %d) '%s'\n", nbytes, buf);

}


void got_msg_from_one_end (int nbytes, char* buf) {

    if (nbytes == sizeof (struct _potato)) {
        throw_potato(buf);
        return;
    }

    printf  ("player: received (length = %d) '%s'\n", nbytes, buf);

}



int server_new_connection (int listener, int my_fdmax, char* remoteIP, socklen_t * addrlen_p, struct sockaddr_storage * remoteaddr_p, fd_set * master_p) {
    *addrlen_p = sizeof (*remoteaddr_p);

    fd_server_RHS = accept(listener, // newly accept()ed socket descriptor
                       (struct sockaddr *)remoteaddr_p,
                       addrlen_p);
    // handle new connections
    if (fd_server_RHS == -1) {
        perror("accept");
    } else {
        FD_SET(fd_server_RHS, master_p); // add to master set
        fdmax = my_fdmax > fd_server_RHS? my_fdmax : fd_server_RHS;
        printf("player: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr_p->ss_family,
                         get_in_addr((struct sockaddr *) remoteaddr_p),
                         remoteIP, INET6_ADDRSTRLEN),
               fd_server_RHS);
    }
    if (fd_server_RHS < 0) printf("ERR: fd_server_RHS err!\n");
    refresh_fd_set();
    return -1;
}

int server_recv_data (int i, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p) {
    printf("ERR: No data should be received here!\n");
    return 0;
}

int player_main_loop () {
    char buf[sizeof potato];    // buffer for client data
    int nbytes;

    refresh_fd_set ();
    // keep track of the biggest file descriptor



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
                return 1; // ending
            }
            if (i == fd_ringmaster) {
                got_msg_from_ringmaster(nbytes, buf);
                continue;
            }
            if (i == fd_client_LHS) {
                got_msg_from_one_end(nbytes, buf);
                continue;
            }
            if (i == fd_server_RHS) {
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

    if (send(fd_ringmaster, PORT, 10, 0) == -1)
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


    input_checker(argc, argv);


    // set fd_server_RHS
    int listener_fd = server_setup (PORT);

    pthread_t t;
    pthread_create(&t, NULL, server_main_loop_helper, (void*)& listener_fd);


    // set client fd from ringmaster
    fd_ringmaster = client_setup (argv[1], argv[2]);

    // register to the ringmaster
    register_to_ringmaster ();

    // main loop
    player_main_loop();

//    pthread_join(t, NULL);

    client_close (fd_ringmaster);


    return 0;
}
