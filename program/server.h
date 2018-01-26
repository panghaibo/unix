//
//  server.h
//  program
//
//  Created by ocean on 18/1/16.
//  Copyright © 2018年 ocean. All rights reserved.
//

#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1

#define MAX_BUFFER_SIZE 2046

typedef struct sockaddr SA;


#endif /* server_h */
