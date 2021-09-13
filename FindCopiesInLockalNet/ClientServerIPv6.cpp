//
// Created by rubik on 10.09.2021.
//

#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include "ClientServerIPv6.h"

void ClientServerIPv6::createSocket() {
    /**
     * Socket creating
     */
    socketFD = socket(AF_INET6, SOCK_DGRAM, 0);
    if(socketFD == ERROR_VALUE) {
        std::cerr << "Error while creating socket";
        exit(EXIT_FAILURE);
    }
    /***
     * Making socket unblockable
     */
    int yes = 1;
    ioctl(socketFD, FIONBIO, &yes);
}

void ClientServerIPv6::setUdpSocket() {
    int yes = 1;
    if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == ERROR_VALUE) {
        std::cerr << "Error while creating SO_REUSEADDR";
        exit(EXIT_FAILURE);
    }
}

void ClientServerIPv6::bindUdpSocket() {
    /**
     * Setting up remote addresses
     *        and then
     * Binding to receive address
     *
     */
    remote.sin6_family  = AF_INET6;
    remote.sin6_port  = htons(port);

    if (bind(socketFD, (sockaddr*) &remote, sizeof(remote)) < 0) {
        std::cerr << "Error while binding";
        exit(EXIT_FAILURE);
    }

    inet_pton(AF_INET6, groupAddr.c_str(), &remote.sin6_addr);

    /**
     * Joining multicast group
     */
    ipv6_mreq mreq{};
    inet_pton(AF_INET6, groupAddr.c_str(), &mreq.ipv6mr_multiaddr);
    mreq.ipv6mr_interface = 0;
    if (setsockopt(socketFD, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*) &mreq, sizeof(mreq)) == ERROR_VALUE){
        std::cerr << "Error while setsockopt";
        exit(EXIT_FAILURE);
    }
}

[[noreturn]] void ClientServerIPv6::run() {

    while (true) {
        sendto(socketFD, uuid, 100, 0,  (sockaddr *)&remote, sizeof(remote));

        std::array<char, 1024> arr{};
        arr.fill(0);
        while(true) {
            if(read(socketFD, arr.data(), arr.size()) == ERROR_VALUE) {
                break;
            }
            message.emplace_back(arr.data());
        }

        for(auto & client : clients) {
            if(client.second == -1) {
                continue;
            }
            client.second--;
        }


        for(auto & it : message) {
            if(clients[it] == -1) {
                continue;
            }
            clients[it] = TTL_VALUE;
        }

        for(auto it = clients.begin(); it != clients.end(); it++) {
            if(it->second == -1) {
                continue;
            } else if (it->second == 0) {
                clients.erase(it++);
            }
        }

        std::cout << "Clients uuids: ";
        for(auto & it : clients) {
            if(it.second == -1) {
                continue;
            }
            std::cout << it.first << " ";
        }
        std::cout << std::endl;
        message.clear();
        sleep(1);
    }
}

ClientServerIPv6::ClientServerIPv6(int port, std::string groupAddr) : ClientServer(port, groupAddr) {
    remote = {};
}
