//
// Created by Brandon on 3/17/2022.
//

#include <stdio.h>
#include <stdlib.h>



int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Arguments usage: player <machine_name> <port_num>\n");
        return -1;
    }


    char* machine_name = atoi(argv[1]);
    char* port_num = atoi(argv[2]);

    printf("machine_name is %d\n", machine_name);
    printf("port_num is %d\n", port_num);

    return 0;
}
