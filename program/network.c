//
//  network.c
//  program
//
//  Created by ocean on 18/1/26.
//  Copyright © 2018年 ocean. All rights reserved.
//

#include "network.h"
#include "server.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "util.h"

int setBlockStatus(int fd, int block)
{
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return FAILURE;
    }
    if (block) {
        flag &= ~O_NONBLOCK;
    } else {
        flag |= O_NONBLOCK;
    }
    if ((flag = fcntl(fd, F_SETFL, flag)) < 0) {
        return FAILURE;
    }
    return SUCCESS;
}

static int setReuseAddr(int fd)
{
    int res, flag = 1;
    res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    return (res == 0) ? SUCCESS : FAILURE;
}

/**
 *
 * 通用 tcp 服务器包裹函数
 *
 */
int generalTcpServer(int af, char *host, int port)
{
    int fd, flag;
    char portStr[6];
    snprintf(portStr, 6, "%d", port);
    struct addrinfo hint, *res, *ptr;
    bzero(&hint, sizeof(hint));
    hint.ai_family = af;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_socktype = SOCK_STREAM;
    if ((flag = getaddrinfo(host, portStr, &hint, &res)) < 0) {
        return FAILURE;
    }
    for (ptr=res; ptr != NULL; ptr = ptr->ai_next) {
        fd = socket(ptr->ai_family, SOCK_STREAM, ptr->ai_protocol);
        if (fd == -1) continue;
        flag = setReuseAddr(fd);
        if (flag == FAILURE) goto error;
        flag = bind(fd, ptr->ai_addr, ptr->ai_addrlen);
        if (flag != 0) goto error;
        flag = listen(fd, 1024); 
        if (flag != 0) goto error;
        goto end;
    }
error:
    fd = FAILURE;
end:
    freeaddrinfo(res);
    return fd;
}


void echoStr(int fd) {
    ssize_t nread;
    char buffer[MAX_BUFFER_SIZE];
    for(;;) {
        nread = read(fd, buffer, MAX_BUFFER_SIZE);
        if (nread < 0) {
            if (errno == EINTR) continue;
            break;
        }
        writen(fd, buffer, nread);
    }
}


/**
 *
 * tcp server
 *
 */
int server() {
    int fd;
    fd = generalTcpServer(AF_INET, "127.0.0.1", 5000);
    if (fd == FAILURE) {
        puts("error");
        return FAILURE;
    }
    int cfd;
    pid_t pid;
    for (;;) {
        cfd = accept(fd, NULL, NULL);
        if (cfd < 0) {
            if (errno ==  EINTR) continue;
            return FAILURE;
        }
        pid = fork();
        if (pid == 0) {
            close(fd);
            echoStr(cfd);
            close(cfd);
        } else {
            close(cfd);
        }
    }
    return SUCCESS;
}




