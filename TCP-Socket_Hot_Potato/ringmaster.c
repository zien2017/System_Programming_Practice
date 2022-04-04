//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "potato.h"
#include "socket_select_server.h"
#include "message_wrapper.h"


int port_num, num_players, num_hops;

struct playerInfo * playerInfo_dummy_head = NULL;
char player_addrs[255][INET6_ADDRSTRLEN];
char player_ports[255][5];
char player_fd_to_addr_mapping[255][INET6_ADDRSTRLEN];

int connected_player = 0;
int ready_player = 0;
int start = 0;

// input checker
int input_parser (int argc, char** argv) {
    if (argc != 4) {
        printf("Arguments usage: ringmaster <port_num> <num_players> <num_hops>\n");
        return (1);
    }

    port_num = atoi(argv[1]);
    num_players = atoi(argv[2]);
    num_hops = atoi(argv[3]);

    if (port_num <= 0 || 65536 < port_num) {
        printf("ERR: port_num error: %d\n", port_num);
        return (1);
    }
    if (num_players <= 0 || 1000 < num_players) {
        printf("ERR: num_players error: %d\n", num_players);
        return (1);
    }
    if (num_hops <= 0 || 65536 < num_hops) {
        printf("ERR: num_hops error: %d\n", num_hops);
        return (1);
    }
    printf("Potato Ringmaster\n");
    printf("Players = %d\n", num_players);
    printf("Hops = %d\n", num_hops);
    return 0;
}

// new connection from clients
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
        inet_ntop(remoteaddr_p->ss_family,
                  get_in_addr((struct sockaddr *) remoteaddr_p),
                  remoteIP, INET6_ADDRSTRLEN),
        // save the new connection ip
        strcpy(player_fd_to_addr_mapping[newfd], remoteIP);

    }

    return fdmax;
}

// assign player with a id
void assign_player_id (int fd, struct playerInfo * player_info) {
    struct register_ret ret;
    ret.player_id = player_info->player_id;
    ret.num_player = num_players;
    wrap_and_send_msg(fd, REGISTER_RET, &ret, sizeof (ret));
    printf("Player %d is ready to play\n", ret.player_id);
}

// add a player and save its information
struct playerInfo *  add_one_player (int fd, char* buf, int nbytes) {

    struct playerInfo * newPlayer = malloc (sizeof (struct playerInfo));
    // add new player info to the header
    newPlayer->next = playerInfo_dummy_head->next;
    playerInfo_dummy_head->next = newPlayer;

    // set info for this node
    newPlayer->fd = fd;
    newPlayer->player_id = connected_player;
    newPlayer->player_port = atoi(buf);
    strcpy(newPlayer->player_addr, player_fd_to_addr_mapping[fd]);

    ++ connected_player;
    return newPlayer;
}

// initialize a ring by telling players their neighbors
void initialize_a_ring (int listener, int fdmax, fd_set * master_p) {
    // create a ring by sending neighbor addresses to players
    start = 1;
    for(int j = 0; j <= fdmax; j++) {
        if (FD_ISSET(j, master_p)) {
            // except the listener
            if (j == listener) {
                continue;
            }
            // find the p info
            struct playerInfo * temp_p = playerInfo_dummy_head->next;
            while (temp_p->fd != j) {
                temp_p = temp_p->next;
            }
            if (temp_p->next == NULL) {
                temp_p = playerInfo_dummy_head;
            }
            // send the player info next to him
            wrap_and_send_msg(j, PLAYER_INFO, temp_p->next, sizeof (struct playerInfo));
        }
    }
}

// setup a potato and send to a random player
void setup_and_throw_a_potato () {
    struct _potato my_potato;
    memset(&my_potato, 0, sizeof (struct _potato));
    my_potato.remaining_counter = num_hops;
    int random = rand() % num_players;
    struct playerInfo * player = playerInfo_dummy_head->next;
    for (int i = 0; i < random; ++ i) {
        player = player->next;
    }
    printf("Ready to start the game, sending potato to player %d\n", player->player_id);
    wrap_and_send_msg(player->fd, POTATO, &my_potato, sizeof (struct _potato));
}

// print the player trace
void print_trace(struct _potato* my_potato) {
    printf("Trace of potato:\n");
    printf("%d", my_potato->player_list[num_hops]);
    for (int i = num_hops - 1; i > 0; -- i) {
        printf(",%d", my_potato->player_list[i]);
    }
    printf("\n");
}

// received a data from player
int server_recv_data (int fd, int fdmax, int listener, char* body_buf, int sizeof_buf, fd_set * master_p) {
    // handle data from a client
    // header buffer
    char header_buf[sizeof(struct msg_header)];
    struct msg_header* h = (struct msg_header *) header_buf;

    if (recv_and_unwrap_msg(fd, body_buf, h) == -1) {
        // got error or connection closed by client
        // connection closed
        printf("ringmaster: socket %d hung up\n", fd);
        close(fd); // bye!
        FD_CLR(fd, master_p); // remove from master set
        return 1; // error exit
    }

    // we got some data from a client

    if (h->type == REGISTER && connected_player < num_players - 1 && start == 0) {
        // add new player
        assign_player_id (fd, add_one_player (fd, body_buf, h->size));
        return 0;
    }
    if (h->type == REGISTER && connected_player == num_players - 1 && start == 0) {
        // start the game
        // add the last player
        assign_player_id (fd, add_one_player (fd, body_buf, h->size));
        // start the game
        initialize_a_ring (listener, fdmax, master_p);
        return 0;
    }
    if (h->type == READY) {
        // got ready info from a player
        ++ ready_player;
        if (ready_player == num_players){
            setup_and_throw_a_potato();
        }
        return 0;
    }
    if (h->type == POTATO) {
        // ending
        print_trace((struct _potato*)body_buf);
        return 1; // escape the forever loop
    }

    printf("ERR: ringmaster: received a unknown msg!\n");

    return 0;

}

// free up the palyer_info helper
void free_up_playInfo (struct playerInfo * curr) {
    if (curr->next != NULL)
        free_up_playInfo(curr->next);
    free(curr);
}

// free up space
void free_up_space () {
    free_up_playInfo (playerInfo_dummy_head);
}

// Arguments usage: ringmaster <port_num> <num_players> <num_hops>
int main(int argc, char** argv) {
    // init
    playerInfo_dummy_head = malloc(sizeof *playerInfo_dummy_head);
    playerInfo_dummy_head->next = NULL;
    playerInfo_dummy_head->fd = 256;

    memset(player_addrs, 0, sizeof player_addrs);
    memset(player_ports, 0, sizeof player_ports);
    memset(player_fd_to_addr_mapping, 0, sizeof player_fd_to_addr_mapping);


    if (input_parser (argc, argv)) {
        free_up_space();
        exit(1);
    }

    srand( (unsigned int) time (NULL) + num_players );

    int listener_fd = server_setup (argv[1]);

    server_main_loop(listener_fd); // the main loop of the server

    close(listener_fd); // close listener fd
    free_up_space();
    return 0;
}


