//
//  unix.c
//  program
//
//  Created by ocean on 18/1/19.
//  Copyright © 2018年 ocean. All rights reserved.
//

#include "unix.h"
#include "server.h"
#include <sys/un.h>
#include <stddef.h>
#define MY_SOCK_PATH "/tmp/shani.sock"

static void deal(int fd)
{
    char buffer[1024];
    ssize_t nread;
    while(1) {
        nread = read(fd, buffer, 1024);
        if (nread < 0) {
            if (errno == EWOULDBLOCK) continue;
            break;
        }
        if (nread == 0) return;
        buffer[nread] = 0;
        write(fd, buffer, nread);
    }
}

void unix_server()
{
    int sfd,cfd;
    pid_t pid;
    socklen_t len;
    struct sockaddr_un addr, client;
    if (access(MY_SOCK_PATH, F_OK) == 0) {
        unlink(MY_SOCK_PATH);
    }
    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd < 0) {
        puts("socket faild");
        return;
    }
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, MY_SOCK_PATH);
    bind(sfd, (SA *)&addr, (offsetof(struct sockaddr_un, sun_path)+strlen(MY_SOCK_PATH)));
    listen(sfd, 2);
    for (;;) {
        cfd = accept(sfd, (SA *)&client, &len);
        if (cfd) {
            if ((pid = fork()) == 0) {
                close(sfd);
                deal(cfd);
                close(cfd);
            } else {
                close(cfd);
            }
        }
    }
}

void unix_client()
{
    int cfd;
    cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, MY_SOCK_PATH);
    connect(cfd, (SA*)&addr, (offsetof(struct sockaddr_un, sun_path)+strlen(MY_SOCK_PATH)));
    char buffer[1024];
    long nread;
    while (fgets(buffer, 1024, stdin) != NULL) {
        write(cfd, buffer, strlen(buffer));
        nread = read(cfd, buffer, 1024);
        buffer[nread] = '\0';
        printf("%s", buffer);
    }
}