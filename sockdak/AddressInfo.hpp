//
//  AddressInfo.hpp
//  sockdak
//
//  Created by Jimmy on 2023/10/28.
//

#ifndef AddressInfo_hpp
#define AddressInfo_hpp

#include <stdio.h>

class AddressInfo {
public:
    bool GetIPAddr(const char *name, struct in_addr *addr);
    bool GetDomainName(struct in_addr addr, char *name, int namelen);
};

#endif /* AddressInfo_hpp */
