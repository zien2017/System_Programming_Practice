//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "potato.h"


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

// Arguments usage: ringmaster <port_num> <num_players> <num_hops>

int main(int argc, char** argv) {
    int port_num, num_players, num_hops;
    input_parser (argc, argv, &port_num, &num_players, &num_hops);
    printf("this is a ringmaster with\n");
    printf("\tport_num is %d\n", port_num);
    printf("\tnum_players is %d\n", num_players);
    printf("\tnum_hops is %d\n", num_hops);

    struct _potato* potato = set_potato (num_hops);

    


}