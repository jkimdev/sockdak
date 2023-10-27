//
//  AddressInfo.cpp
//  sockdak
//
//  Created by Jimmy on 2023/10/28.
//

#include "AddressInfo.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

bool AddressInfo::GetIPAddr(const char *name, struct in_addr *addr) {
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

bool AddressInfo::GetDomainName(struct in_addr addr, char *name, int namelen) {
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
