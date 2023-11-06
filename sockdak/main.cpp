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

const char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, const char * argv[]) {
    int retval;
    
    if (argc > 1) SERVERIP = argv[1];
    
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) err_quit("socket()");
    
    // connect
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) err_quit("connect()");
    
    // communication variables
    char buf[BUFSIZE + 1];
    int len;
    
    // client to server
    while (1) {
        printf("\n[type:]");
        if (fgets(buf, BUFSIZE, stdin) == NULL)
            break;
        
        // remove '\n'
        len = (int)strlen(buf);
        if(buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        if(strlen(buf) == 0)
            break;
        
        // send data
        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == -1) {
            err_display("send()");
            break;
        }
        
        printf("[TCP Client] %d bytes has been sent", retval);
        
        // reveice data
        retval = recv(sock, buf, retval, MSG_WAITALL);
        if (retval == -1) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;
        
        // print received data
        buf[retval] = '\0';
        printf("[TCP Client] %d bytes has been received", retval);
        printf("[data] %s\n", buf);
        
        close(sock);
    }
}
