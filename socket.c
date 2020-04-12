//
//  socket.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/10.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "socket.h"

int create_socket(void) {
    int socket_fd;

    // create TCP connection
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void init_socket(struct sockaddr_in *socket, char *socket_addr) {
    socket->sin_family = AF_INET;
    socket->sin_port = htons(PORT_NUM);

    if (socket_addr) {
        socket->sin_addr.s_addr = inet_addr(socket_addr);
        inet_aton(socket_addr, (struct in_addr *) &(socket->sin_addr.s_addr));
    }
    else {
        socket->sin_addr.s_addr = INADDR_ANY;
    }
}

void connect_server(int socketFD, struct sockaddr_in *info) {
    if (connect(socketFD, (struct sockaddr *) info, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}
