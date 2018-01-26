//
//  util.c
//  program
//
//  Created by ocean on 18/1/25.
//  Copyright © 2018年 ocean. All rights reserved.
//

#include "util.h"
#include <errno.h>
#include <unistd.h>

ssize_t readn(int fd, char *buffer, ssize_t maxlen)
{
    ssize_t left, nread;
    left = maxlen;
    while (left > 0) {
        nread = read(fd, buffer, left);
        if (nread == -1) {
            if (errno == EINTR) continue;
            return -1;
        } else if (nread == 0) {
            break;
        }
        buffer += nread;
        left -= nread;
    }
    return maxlen - left;
}

ssize_t writen(int fd, char *buffer, ssize_t maxlen)
{
    ssize_t left, nwrite;
    left = maxlen;
    while (left > 0) {
        nwrite = write(fd, buffer, left);
        if (nwrite == -1) {
            if(errno == EINTR) continue;
            return -1;
        }
        buffer += nwrite;
        left -= nwrite;
    }
    return maxlen;
}

ssize_t readline(int fd, char *buffer, ssize_t maxlen)
{
    ssize_t left, nread;
    left = maxlen;
    char tmp, *ptr;
    ptr = buffer;
    while(left > 0) {
        nread = read(fd, &tmp, 1);
        if (nread == -1) {
            if(errno == EINTR) continue;
            return -1;
        } else if(nread == 0) {
            break;
        } else if(tmp == '\n') {
            break;
        }
        *ptr++ = tmp;
        left--;
    }
    *ptr = 0;
    return maxlen - left;
}

