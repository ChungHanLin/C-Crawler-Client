//
//  socket.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/10.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef socket_h
#define socket_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "struct.h"

#define PORT_NUM 8080
#define BUFFER_SIZE 1024
#define BACKLOG_NUM 10

int create_socket(void);

void init_socket(struct sockaddr_in *socket, char *socket_addr);

void connect_server(int socketFD, struct sockaddr_in *info);

#endif /* socket_h */
