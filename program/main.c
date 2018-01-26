//
//  main.c
//  program
//
//  Created by ocean on 18/1/17.
//  Copyright © 2018年 ocean. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "unix.h"
#include "network.h"
#include "server.h"

int main(int argc, char *argv[])
{
    server();
    return 0;
}