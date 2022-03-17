//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Arguments usage: ringmaster <port_num> <num_players> <num_hops>\n");
        return -1;
    }

    char* port_num = atoi(argv[1]);
    char* num_players = atoi(argv[2]);
    char* num_hops = atoi(argv[3]);

    printf("port_num is %d\n", port_num);
    printf("num_players is %d\n", num_players);
    printf("num_hops is %d\n", num_hops);

    return 0;
}
