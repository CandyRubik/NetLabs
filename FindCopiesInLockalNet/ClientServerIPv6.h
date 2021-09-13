//
// Created by rubik on 10.09.2021.
//

#ifndef LAB1_CLIENTSERVERIPV6_H
#define LAB1_CLIENTSERVERIPV6_H


#include <netinet/in.h>
#include "ClientServer.h"

class ClientServerIPv6 : public ClientServer{
private:
    sockaddr_in6 remote;
public:
    ClientServerIPv6(int port, std::string groupAddr);

    void createSocket() override;

    void setUdpSocket() override;

    void bindUdpSocket() override;

    [[noreturn]] void run() override;
};


#endif //LAB1_CLIENTSERVERIPV6_H
