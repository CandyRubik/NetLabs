#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <array>

#define MSGBUFSIZE 128
#include "Server.h"


Server::~Server() {
    close(sockfd);
}

Server::Server(int port, std::string multicast_group) {

    // create what looks like an ordinary UDP socket
    //
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        std::cerr << "Error while creating socket";
        exit(EXIT_FAILURE);
    }

    // allow multiple sockets to use the same PORT number
    //
    int yes = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        std::cerr << "Error while creating socket";
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(stSockAddr));

    // set up destination address
    //
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(4321);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    // bind to receive address
    //
    if (bind(sockfd, (struct sockaddr*) &stSockAddr, sizeof(stSockAddr)) < 0) {
        std::cerr << "Error while binding";
        exit(EXIT_FAILURE);
    }


    // use setsockopt() to request that the kernel join a multicast group
    //
    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
    mreq.imr_interface.s_addr = inet_addr("127.0.0.1");
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) == -1){
        std::cerr << "Error while setsockopt";
        exit(EXIT_FAILURE);
    }

    // Read from the socket
    std::array<char, 1024> arr;
    arr.fill(0);
    read(sockfd, arr.data(), arr.size());
    std::cout << "Message from multicast sender: " << arr.data()
              << std::endl;
}

void Server::setUUID(int id) {
    uuid = id;
}

void Server::run() {
    // now just enter a read-print loop
    //
    while (1) {
        char msgbuf[MSGBUFSIZE];
        int addrlen = sizeof(stSockAddr);
        int nbytes = recv(
                sockfd,
                msgbuf,
                MSGBUFSIZE,
                0);
        if (nbytes < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        msgbuf[nbytes] = '\0';
        puts(msgbuf);
    }
}
