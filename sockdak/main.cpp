//
//  main.cpp
//  sockdak
//
//  Created by Jimmy on 2023/10/19.
//

#include <iostream>
#include "Common.h"
#include "AddressInfo.cpp"


using namespace std;

int socket(
           int domain,
           int type,
           int protocol
           );

int close(int fd);



int main(int argc, const char * argv[]) {
    
    // insert code here...
    AddressInfo addressInfo;
    const char *TESTNAME = "www.google.com";
    struct in_addr addr;
    
    if (addressInfo.GetIPAddr(TESTNAME, &addr)) {
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, str, sizeof(str));
        cout << str << endl;
    }
    
    char name[256];
    if (addressInfo.GetDomainName(addr, name, sizeof(name))) {
        cout << name << endl;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) err_quit("socket()");
    printf("[notice] socket initialized successfully!");
    
    close(sock);
    return 0;
}
