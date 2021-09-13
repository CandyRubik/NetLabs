//
// Created by rubik on 10.09.2021.
//

#ifndef LAB1_CLIENTSERVER_H
#define LAB1_CLIENTSERVER_H

#define ERROR_VALUE -1
#define TTL_VALUE 2
#include <string>
#include <vector>
#include <map>

class ClientServer {
public:
    ClientServer(int port, std::string groupAddr);

    virtual void createSocket() = 0;

    virtual void setUdpSocket() = 0;

    virtual void bindUdpSocket() = 0;

    [[noreturn]] virtual void run() = 0;

    virtual  ~ClientServer();

protected:
    std::map<std::string, int> clients; /* value of map works like TTL by default is 5, also { uuid , -1 } it's our values for defining yourself */
    int socketFD;
    std::string groupAddr;
    int port;
    char uuid[100];
    std::vector<std::string> message;
};


#endif //LAB1_CLIENTSERVER_H
