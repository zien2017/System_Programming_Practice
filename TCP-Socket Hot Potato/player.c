//
// Created by Brandon on 3/17/2022.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "socket_client.h"
#include "pthread.h"

int sockfd_client_for_ringmaster = 0;
int sockfd_client_ring_left = 0;
int sockfd_server_ring_right = 0;

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


void client_business_for_master (int sockfd) {
    char recv_buf[MAXDATASIZE];
    char send_buf[100] = "msg snum =  \0";

    // send
    for (int counter = 5; counter > 0; -- counter) {
        send_buf[11] = (char) (counter + '0');

        if (send(sockfd, send_buf, 13, 0) == -1)
            perror("send");
        printf("client: sent %s\n", send_buf);
    }

    // recv
    int numbytes;
    while ((numbytes = recv(sockfd, recv_buf, MAXDATASIZE - 1, 0)) != -1) {
        recv_buf[numbytes] = '\0';
        for (int temp = 0; temp < numbytes; temp += 13) {
            printf("client: received (length = %d) '%s'\n", numbytes, recv_buf + temp);
            if (numbytes - temp == 1) {
                close(sockfd);
                printf("client: closed by server.");
                return;
            }
        }
        memset(recv_buf, 0, sizeof(recv_buf));
    }
    close(sockfd);
}


void client_business_for_ring (int sockfd) {

}

void client_business (int sockfd) {
    if (sockfd == sockfd_client_for_ringmaster)
        client_business_for_master (sockfd);

    if (sockfd == sockfd_client_ring_left)
        client_business_for_ring (sockfd);
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


    sockfd_client_for_ringmaster = client_setup (argv[1], argv[2]);

    client_business(sockfd_client_for_ringmaster);

    client_close (sockfd_client_for_ringmaster);




    return 0;
}
