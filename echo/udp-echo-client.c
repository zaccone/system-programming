#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEN 1024

int main(int argc, char* argv[]) {

   /*
    ./udp-echo-client ip port
    */

    uint16_t port = 4321;
    char* address = "127.0.0.1";
    switch(argc) {
    case 4:
    case 3:
        port = atoi(argv[2]);
    case 2:
        address = argv[1];

   };

    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
         (const void *)&optval , sizeof(int));


    struct sockaddr_in serveraddr, clientaddr;
    struct in_addr ip4;
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    memset((char *) &clientaddr, 0, sizeof(clientaddr));
    serveraddr.sin_family = PF_INET;
    inet_pton(AF_INET, address,&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(port);
    socklen_t slen = sizeof(serveraddr);


    int send_res;
    while(1) {
        if(sendto(sock, "DUPA!", sizeof("DUPA!"), 0,
                (struct sockaddr*) &serveraddr,slen) < 0 ) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
