//
//  util.h
//  program
//
//  Created by ocean on 18/1/25.
//  Copyright © 2018年 ocean. All rights reserved.
//

#ifndef util_h
#define util_h
#include <sys/types.h>

ssize_t readn(int fd, char *buffer, ssize_t maxlen);
ssize_t writen(int fd, char *buffer, ssize_t maxlen);
ssize_t readline(int fd, char *buffer, ssize_t maxlen);

#endif /* util_h */
