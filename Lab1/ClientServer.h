//
// Created by rubik on 09.09.2021.
//

#ifndef LAB1_CLIENTSERVER_H
#define LAB1_CLIENTSERVER_H


#include <netinet/in.h>
#include <string>

class ClientServer {
private:
    int socketFD;
    sockaddr_in remote;
    std::string group_addr;
    int port;
    std::string uuid;
public:
    ClientServer(int port, std::string group_addr, std::string uuid);
    ClientServer createSocket();
    ClientServer setUdpSocket();
    ClientServer bindUdpSocket();

    void run();
};


#endif //LAB1_CLIENTSERVER_H
