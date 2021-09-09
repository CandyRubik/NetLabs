//
// Created by Владимир on 08.09.2021.
//

#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "Client.h"

Client::Client(int port, std::string multicast_group) {
    // create what looks like an ordinary UDP socket
    //
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

}

void Client::run() {
    // set up destination address
    //
    sockaddr_in groupSock = {};   // init to all zeroes
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
    groupSock.sin_port = htons(4321);

    in_addr localIface = {};   // init to all zeroes
    localIface.s_addr = inet_addr("127.0.0.1");
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localIface,
               sizeof(localIface));

    const std::string databuf = "Multicast from C++";
    sendto(sockfd, databuf.c_str(), databuf.length(), 0,
           (sockaddr*)&groupSock, sizeof(groupSock));
}

Client::~Client() {
    close(sockfd);
}
