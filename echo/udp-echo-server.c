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
    ./udp-echo-client <ip> <port>
    */

    uint16_t port = 4321;
    char* address = "127.0.0.1";

    char buf[LEN];

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


    struct sockaddr_in serveraddr;
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = PF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    // bind to an address

    int b_ok = bind(sock, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
    if (b_ok < 0) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_storage fromaddr;
    socklen_t len = sizeof(fromaddr);
    memset(&fromaddr, 0, len);
    int counter = 1024;
    while(1) {
        // iteratively wait for datagrams and echo back the output
        memset(&buf, 0, sizeof(buf));
        int size = recvfrom(sock, buf, sizeof(buf),0, (struct sockaddr*) &fromaddr, &len);
        if ((counter % 1024) == 0) {
            printf(" %d) Received data: %s\n", counter, buf);
            fflush(stdout);
        }
        counter++;
    }

    return 0;
}
