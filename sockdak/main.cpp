//
//  main.cpp
//  sockdak
//
//  Created by Jimmy on 2023/10/19.
//

#include <iostream>
#include "Common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int socket(
           int domain,
           int type,
           int protocol
           );

int close(int fd);

int main(int argc, const char * argv[]) {
    // insert code here...
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) err_quit("socket()");
    printf("[notice] socket initialized successfully!");
    
    close(sock);
    return 0;
}
