//
// Created by rubik on 09.09.2021.
//

#ifndef LAB1_CLIENTSERVERIPV4_H
#define LAB1_CLIENTSERVERIPV4_H

#include <netinet/in.h>
#include <string>
#include <vector>
#include <map>
#include "ClientServer.h"

class ClientServerIPv4 : public ClientServer{
private:
    sockaddr_in remote;
public:
    ClientServerIPv4(int port, std::string groupAddr);

    void createSocket() override;

    void setUdpSocket() override;

    void bindUdpSocket() override;

    [[noreturn]] void run() override;
};


#endif //LAB1_CLIENTSERVERIPV4_H
