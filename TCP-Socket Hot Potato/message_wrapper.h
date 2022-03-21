//
// Created by Brandon Wong on 2022/3/21.
//

#ifndef TCP_SOCKET_HOT_POTATO_MESSAGE_WRAPPER_H
#define TCP_SOCKET_HOT_POTATO_MESSAGE_WRAPPER_H
#include "potato.h"

enum msg_type {
    DEFAULT,
    STR,
    REGISTER,
    POTATO,
    PLAYER_INFO,
    REGISTER_RET,
    READY,
    ACK,
};

struct msg_header {
    int size;
    enum msg_type type;
};

struct register_ret {
    int player_id;
    int num_player;
};

struct playerInfo {
    char player_addr[INET6_ADDRSTRLEN];
    int player_port;
    int player_id;
    int fd;
    struct playerInfo * next;
} ;

void wrap_and_send_msg (int fd, enum msg_type type, void* msg_body, int size) {
    char temp_buf[sizeof (struct msg_header) + size];

    struct msg_header header;
    header.size = size;
    header.type = type;

    memcpy(temp_buf, &header, sizeof (struct msg_header));
    if (msg_body != NULL){
        memcpy(temp_buf + sizeof (struct msg_header), msg_body, size);
    }

    if (send(fd, temp_buf, sizeof(temp_buf), 0) == -1)
        perror("MSG sending failure ");
}

int recv_and_unwrap_msg (int fd, void* msg_body, struct msg_header *header) {
    char temp_buf[sizeof (struct msg_header) + sizeof(struct _potato)];
    int total_size = 0;

    if ((total_size = recv(fd, temp_buf, sizeof (temp_buf), 0)) <= 0) {
        // got error or connection closed by client
        if (total_size == 0) {
            // connection closed
//            printf("socket %d hung up\n", fd);
            close(fd);
        } else {
            perror("recv");
        }
        return -1;
    }
    memcpy(header, temp_buf, sizeof (struct msg_header));
    if (header->size > 0){
        memcpy(msg_body, temp_buf + sizeof (struct msg_header), header->size);
    }
    if (total_size - sizeof (struct msg_header) != header->size) {
        fprintf(stderr, "ERR: wrapper: incomplete msg received! from %d \n", fd);
        close(fd);
        return -1;
    }

    return header->size;
}

#endif //TCP_SOCKET_HOT_POTATO_MESSAGE_WRAPPER_H
