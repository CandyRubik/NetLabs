//
// Created by rubik on 09.09.2021.
//

#include <iostream>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <array>
#include <unistd.h>
#include "ClientServerIPv4.h"

ClientServerIPv4::ClientServerIPv4(int port, std::string groupAddr) : ClientServer(port, groupAddr) {
    remote = {};
}

[[noreturn]] void ClientServerIPv4::run() {
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

void ClientServerIPv4::createSocket() {
    /**
     * Socket creating
     */
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
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

void ClientServerIPv4::setUdpSocket() {
    int yes = 1;
    if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == ERROR_VALUE) {
        std::cerr << "Error while creating SO_REUSEADDR";
        exit(EXIT_FAILURE);
    }

}

void ClientServerIPv4::bindUdpSocket() {

    /**
     * Setting up remote addresses
     *        and then
     * Binding to receive address
     *
     */
    remote.sin_family  = AF_INET;
    remote.sin_port  = htons(port);
    remote.sin_addr.s_addr = inet_addr(groupAddr.c_str());

    if (bind(socketFD, (sockaddr*) &remote, sizeof(remote)) < 0) {
        std::cerr << "Error while binding";
        exit(EXIT_FAILURE);
    }



    /**
     * Joining multicast group
     */
    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(groupAddr.c_str());
    mreq.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(socketFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) == ERROR_VALUE){
        std::cerr << "Error while setsockopt";
        exit(EXIT_FAILURE);
    }

}
