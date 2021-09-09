//
// Created by rubik on 09.09.2021.
//
#define BAD_VALUE -1

#include <iostream>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <array>
#include <unistd.h>

#include "ClientServer.h"

ClientServer::ClientServer(int port, std::string group_addr, std::string uuid) {

    this->uuid = uuid;
    this->group_addr = group_addr;
    this->port = port;
}

void ClientServer::run() {
    while (true) {
        const std::string databuf = "Multicast from C++" + uuid;
        sendto(socketFD, databuf.c_str(), databuf.length(), 0,
               (sockaddr*)&remote, sizeof(remote));

        std::array<char, 1024> arr;
        arr.fill(0);
        read(socketFD, arr.data(), arr.size());
        std::cout << "Message from multicast sender: " << arr.data()
        << std::endl;

        sleep(1);
    }
}

ClientServer ClientServer::createSocket() {
    /**
     * Socket creating
     */
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketFD == BAD_VALUE) {
        std::cerr << "Error while creating socket";
        exit(EXIT_FAILURE);
    }
    /***
     * Making socket unblockable
     */
    int yes = 1;
    ioctl(socketFD, FIONBIO, &yes);

    return *this;
}

ClientServer ClientServer::setUdpSocket() {
    int yes = 1;
    if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == BAD_VALUE) {
        std::cerr << "Error while creating SO_REUSEADDR";
        exit(EXIT_FAILURE);
    }

    /**
     * Setting up remote addresses
     */

    remote = {};
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    remote.sin_addr.s_addr = inet_addr(group_addr.c_str());

    return *this;
}

ClientServer ClientServer::bindUdpSocket() {

    /**
     * Binding to receive address
     */
    if (bind(socketFD, (sockaddr*) &remote, sizeof(remote)) < 0) {
        std::cerr << "Error while binding";
        exit(EXIT_FAILURE);
    }

    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group_addr.c_str());
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(socketFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) == -1){
        std::cerr << "Error while setsockopt";
        exit(EXIT_FAILURE);
    }

    return *this;
}
