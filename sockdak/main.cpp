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
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

int socket(
           int domain,
           int type,
           int protocol
           );

int close(int fd);

bool GetIPAddr(const char *name, struct in_addr *addr) {
    struct hostent *ptr = gethostbyname(name);
    
    if (ptr == NULL) {
        err_display("gethostbyname()");
        return false;
    }
    if (ptr -> h_addrtype != AF_INET)
        return false;
    memcpy(addr, ptr -> h_addr, ptr -> h_length);
    return true;
}

bool GetDomainName(struct in_addr addr, char *name, int namelen) {
    struct hostent *ptr = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
    
    if (ptr == NULL) {
        err_display("gethostbyaddr()");
        return false;
    }
    if (ptr -> h_addrtype != AF_INET)
        return false;
    strncpy(name, ptr->h_name, namelen);
    return true;
}

int main(int argc, const char * argv[]) {
    
    // insert code here...
    const char *TESTNAME = "www.google.com";
    struct in_addr addr;
    
    if (GetIPAddr(TESTNAME, &addr)) {
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, str, sizeof(str));
        cout << str << endl;
    }
    
    char name[256];
    if (GetDomainName(addr, name, sizeof(name))) {
        cout << name << endl;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) err_quit("socket()");
    printf("[notice] socket initialized successfully!");
    
    
    close(sock);
    return 0;
}
