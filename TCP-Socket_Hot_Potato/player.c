//
// Created by Brandon on 3/17/2022.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "potato.h"
#include "socket_client.h"
#include "message_wrapper.h"

#define BUFFER_SIZE sizeof (struct _potato ) + sizeof (struct msg_header)


fd_set master, read_fds;
int fdmax = 10;
int player_id = -1;
int num_player = -1;
int left_player = -1;
int right_player = -1;
char my_server_port[6];

int listener_fd;
int fd_ringmaster = -1;
int fd_client_LHS = -1;
int fd_server_RHS = -1;

int has_sent_ready = 0;


void refresh_fd_set () {
//    printf("refreshing fd: %d %d %d %d\n", listener_fd, fd_ringmaster, fd_client_LHS, fd_server_RHS);
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // add the fds to the master set
    fdmax = fdmax > listener_fd ? fdmax : listener_fd;
    fdmax = fdmax > fd_ringmaster ? fdmax : fd_ringmaster;
    fdmax = fdmax > fd_client_LHS ? fdmax : fd_client_LHS;
    fdmax = fdmax > fd_server_RHS ? fdmax : fd_server_RHS;

    if (listener_fd > 0)
        FD_SET(listener_fd, &master);
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
//
//    char* machine_name = argv[1];
    int port_num = atoi(argv[2]);
    if (port_num <= 0 || 65536 < port_num) {
        printf("ERR: port_num error: %d\n", port_num);
        exit(1);
    }

}

// send ready when all connection from left and right are connected
void send_ready () {
    if (has_sent_ready == 0 && fd_client_LHS > 0 && fd_server_RHS > 0) {
        wrap_and_send_msg(fd_ringmaster, READY, NULL, 0);
        has_sent_ready = 1;
    }
}



int server_setup() {

    int listener;     // listening socket descriptor
    int yes = 1;        // for setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, "0", &hints, &ai)) != 0) {
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

        // get random port and rewrite back to char* port
        struct sockaddr_in addr;
        int size=sizeof(addr);
        getsockname(listener, (void*) &addr, (socklen_t *) & size);
        int new_port = ntohs( addr.sin_port);

        sprintf(my_server_port, "%d", new_port);

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

void throw_potato(char* buf ){
    // received a potato
    struct _potato * p = (struct _potato *) buf;
    // add my name on potato
    p->player_list[p->remaining_counter] = player_id;

    // throw potato
    if (p->remaining_counter == 0) {
        // ending give potato back to the ringmaster
        printf("I’m it\n");
        wrap_and_send_msg(fd_ringmaster, POTATO, p, sizeof (struct _potato));
    } else {
        -- p->remaining_counter;
        // next hop give back to ringmaster
        if (rand() % 2 ) {
            printf("Sending potato to %d\n", right_player);
            wrap_and_send_msg(fd_server_RHS, POTATO, p, sizeof (struct _potato));
        } else {
            printf("Sending potato to %d\n", left_player);
            wrap_and_send_msg(fd_client_LHS, POTATO, p, sizeof (struct _potato));
        }
    }
}

void connect_to_adjacent_player (char* buf) {
    // assigned to set up a ring
    struct playerInfo* p_info = (struct playerInfo*) buf;

    char port_str[6];
    sprintf(port_str, "%d", p_info->player_port);

    char remoteIP[INET6_ADDRSTRLEN];
    strcpy(remoteIP, p_info->player_addr);

    // set fd_client_LHS
    fd_client_LHS = client_setup (remoteIP, port_str);

    refresh_fd_set();

    send_ready();
}


void add_new_conn () {
    struct sockaddr_storage remoteaddr; // client address
    memset(&remoteaddr, 0, sizeof remoteaddr);


//    char buf[BUFFER_SIZE];    // buffer for client data
//    char remoteIP[INET6_ADDRSTRLEN];

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    socklen_t addrlen = sizeof (remoteaddr);

    fd_server_RHS = accept(listener_fd, // newly accept()ed socket descriptor
                           (struct sockaddr *)&remoteaddr,
                           &addrlen);
    // handle new connections
    if (fd_server_RHS < 0) {
        perror("accept");
        printf("ERR: fd_server_RHS err! id = %d\n", player_id);
        printf("%d\n", errno);
    } else {
        FD_SET (fd_server_RHS, &master); // add to master set
        fdmax = fdmax > fd_server_RHS? fdmax : fd_server_RHS;
    }


    refresh_fd_set();

    send_ready ();
}

int player_main_loop () {
    char buf[BUFFER_SIZE];    // buffer for client data

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
        for(int fd = 0; fd <= fdmax; fd++) {
            // we got one!!
            if ( ! FD_ISSET(fd, &read_fds)) {
                continue;
            }

            if (fd == listener_fd) {
                add_new_conn();
                continue;
            }

            // header buffer
            char header_buf[sizeof(struct msg_header)];
            struct msg_header* h = (struct msg_header *) header_buf;

            if (recv_and_unwrap_msg(fd, buf, h) == -1) {
                // got error or connection closed by client
                // connection closed
                close(fd); // bye!
                FD_CLR(fd, &master); // remove from master set
                return 1; // error exit
            }

            if (h->type == STR) {
                buf[h->size] = '\0';
                printf("Received str: %s\n", buf);
                continue;
            }
            if (h->type == REGISTER_RET) {
                struct register_ret ret = *((struct register_ret*)buf);
                num_player = ret.num_player;
                player_id = ret.player_id;
                left_player = player_id - 1 >= 0? player_id - 1 : num_player - 1;
                right_player = player_id + 1 < num_player? player_id + 1 : 0;
                printf("Connected as player %d out of %d total players\n", player_id, num_player);
                continue;
            }
            if (h->type == PLAYER_INFO && fd == fd_ringmaster) {
                connect_to_adjacent_player(buf);
                continue;
            }
            if (h->type == POTATO) {
//                printf("potato received! (id = %d)\n", player_id);
                throw_potato(buf);
                continue;
            }


            printf("ERR: player: received a unknown msg !\n");
            printf("\tmsg from fd: %d!\n", fd);
            printf("\tmsg type: %s, size: %d\n", (char*)&h->type, h->size);

            // END handle data from client

        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}



void register_to_ringmaster () {
    wrap_and_send_msg (fd_ringmaster, REGISTER, my_server_port, 6);
//    printf("\tmy port is %s\n", PORT);
}


/*
 * Arguments usage: player <machine_name> <port_num>
 * where machine_name is the machine name (e.g. dku-vcm-508.vm.duke.edu) where the ringmaster
 * process is running and port_num is the port number given to the ringmaster process
 */
int main(int argc, char** argv) {
    // argv[1] machine name of ringmaster
    // argv[2] port_num of ringmaster

    memcpy(my_server_port, "30003", 6);

    input_checker(argc, argv);

    srand( (unsigned int) time (NULL) + player_id );


    // set fd_server_RHS
    listener_fd = server_setup ();

    // set client fd from ringmaster
    fd_ringmaster = client_setup (argv[1], argv[2]);

    // register to the ringmaster
    register_to_ringmaster ();

    // main loop
    player_main_loop();


    client_close (fd_ringmaster);


    return 0;
}
