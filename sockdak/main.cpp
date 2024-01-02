//
//  main.cpp
//  sockdak
//
//  Created by Jimmy on 2023/10/19.
//

#include "client.hpp"

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 9000

int main(int argc, const char * argv[]) {
    Client chat(SERVER_ADDR, SERVER_PORT);
    chat.Start();

    return 0;
}
