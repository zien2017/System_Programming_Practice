//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "potato.h"
#include "socket_select_server.h"


int port_num, num_players, num_hops;

struct playerInfo {
    char player_addr[INET6_ADDRSTRLEN];
    int player_port;
    int player_fd;
    struct playerInfo * next;
} PlayerInfoNode;;

struct playerInfo * playerInfo_dummy_head = NULL;
char player_addrs[255][INET6_ADDRSTRLEN];
char player_ports[255][5];
char player_fd_to_addr_mapping[255][INET6_ADDRSTRLEN];



int connected_player = 0;
int begin = 0;

// input checker
void input_parser (int argc, char** argv) {
    if (argc != 4) {
        printf("Arguments usage: ringmaster <port_num> <num_players> <num_hops>\n");
        exit(1);
    }

    port_num = atoi(argv[1]);
    num_players = atoi(argv[2]);
    num_hops = atoi(argv[3]);

    if (port_num <= 0 || 65536 < port_num) {
        printf("ERR: port_num error: %d\n", port_num);
        exit(1);
    }
    if (num_players <= 0 || 300 < num_players) {
        printf("ERR: num_players error: %d\n", num_players);
        exit(1);
    }
    if (num_hops <= 0 || 65536 < num_hops) {
        printf("ERR: num_hops error: %d\n", num_hops);
        exit(1);
    }
    printf("this is a ringmaster with\n");
    printf("\tport_num is %d\n", port_num);
    printf("\tnum_players is %d\n", num_players);
    printf("\tnum_hops is %d\n", num_hops);

}

// malloc a new potato
struct _potato * set_potato () {
    struct _potato * potato = malloc(sizeof(struct _potato));
    memset(potato, 0, sizeof(struct _potato));
    return potato;
}

int server_new_connection (int listener, int fdmax, char* remoteIP, socklen_t * addrlen_p, struct sockaddr_storage * remoteaddr_p, fd_set * master_p) {
    *addrlen_p = sizeof (*remoteaddr_p);

    int newfd = accept(listener, // newly accept()ed socket descriptor
                       (struct sockaddr *) remoteaddr_p,
                       addrlen_p);

    // handle new connections
    if (newfd == -1) {
        perror("accept");
    } else {
        FD_SET(newfd, master_p); // add to master set
        if (newfd > fdmax) {    // keep track of the max
            fdmax = newfd;
        }
        printf("ringmaster: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr_p->ss_family,
                         get_in_addr((struct sockaddr *) remoteaddr_p),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);

        // save the new connection ip
        memcpy(player_fd_to_addr_mapping[newfd], remoteIP, *addrlen_p);

    }


    return fdmax;
}

void add_one_player (int fd, char* buf, int nbytes) {
    // add a player
    ++ connected_player;
    struct playerInfo * newPlayer = malloc (sizeof (struct playerInfo));
    // add new player info to the header
    newPlayer->next = playerInfo_dummy_head->next;
    playerInfo_dummy_head->next = newPlayer;


    // set info for this node
    newPlayer->player_fd = fd;
    newPlayer->player_port = atoi(buf);
    memcpy(& newPlayer->player_addr, player_fd_to_addr_mapping[fd], INET6_ADDRSTRLEN);
    printf("ringmaster: recv (%d bytes): %s\n", nbytes, buf);
    printf("\tcreate new player %s:%d\n", newPlayer->player_addr, newPlayer->player_port);
    printf("\t#connected_player = %d\n", connected_player);

}

void initialize_a_ring (int listener, int fdmax, fd_set * master_p) {
    // create a ring by sending neighbor addresses to players
    begin = 1;
    printf("creating rings ... \n");
    for(int j = 0; j <= fdmax; j++) {
        if (FD_ISSET(j, master_p)) {
            // except the listener
            if (j == listener) {
                continue;
            }
            // find the p info
            struct playerInfo * temp_p = playerInfo_dummy_head->next;
            while (temp_p->player_fd != j) {
                temp_p = temp_p->next;
            }
            if (temp_p->next == NULL) {
                temp_p = playerInfo_dummy_head;
            }
            // send the player info next to him
            printf("send port %d to fd %d\n", temp_p->next->player_port, temp_p->next->player_fd);
            if (send(j, temp_p->next, sizeof(struct playerInfo), 0) == -1) {
                perror("ERR: send");
            }
        }
    }
}

void throw_a_potato () {
    struct potato * po = malloc(sizeof (struct _potato));
    if (send(playerInfo_dummy_head->next->player_fd, po, sizeof (struct _potato), 0) == -1) {
        perror("ERR: send\n");
    }
    printf("sent a potato\n");
}

void server_recv_data (int fd, int nbytes, int fdmax, int listener, char* buf, int sizeof_buf, fd_set * master_p) {
    // handle data from a client
    if ((nbytes = recv(fd, buf, sizeof_buf, 0)) <= 0) {
        // got error or connection closed by client
        if (nbytes == 0) {
            // connection closed
            printf("ringmaster: socket %d hung up\n", fd);
        } else {
            perror("recv");
        }
        close(fd); // bye!
        FD_CLR(fd, master_p); // remove from master set
    } else {
        // we got some data from a client

        if (connected_player < num_players) {
            add_one_player (fd, buf, nbytes) ;
        }

        if (connected_player == num_players &&  begin == 0) {
            initialize_a_ring (listener, fdmax, master_p);
            sleep(1);
            throw_a_potato ();
        } else {
            // ending


        }


//        for(int j = 0; j <= fdmax; j++) {
//            // send to everyone!
//            if (FD_ISSET(j, master_p)) {
//                // except the listener and ourselves
//                if (j != listener && j != fd) {
//                    if (send(j, buf, nbytes, 0) == -1) {
//                        perror("send");
//                    }
//                }
//            }
//        }
    }
}

void server_close(int socket_fd) {
    close(socket_fd);
}




// Arguments usage: ringmaster <port_num> <num_players> <num_hops>
int main(int argc, char** argv) {
    // init
    playerInfo_dummy_head = malloc(sizeof *playerInfo_dummy_head);
    playerInfo_dummy_head->next = NULL;
    playerInfo_dummy_head->player_fd = 256;

    memset(player_addrs, 0, sizeof player_addrs);
    memset(player_ports, 0, sizeof player_ports);
    memset(player_fd_to_addr_mapping, 0, sizeof player_fd_to_addr_mapping);


    input_parser (argc, argv);



    struct _potato* potato = set_potato ();


    int listener_fd = server_setup (argv[1]);

    server_main_loop(listener_fd);

    server_close(listener_fd);

    return 0;
}


