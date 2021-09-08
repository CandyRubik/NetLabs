//
// Created by Владимир on 08.09.2021.
//

#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "Client.h"

Client::Client(int port, std::string multicast_group) {
    // create what looks like an ordinary UDP socket
    //
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }

    // set up destination address
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(multicast_group.c_str());
    addr.sin_port = htons(port);
}

void Client::run() {
    while (1) {
        char ch = 0;
        int nbytes = sendto(
                sockfd,
                message,
                strlen(message),
                0,
                (struct sockaddr*) &stSockAddr,
                sizeof(stSockAddr)
        );
        if (nbytes < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(delay_secs); // Unix sleep is seconds
    }
}
