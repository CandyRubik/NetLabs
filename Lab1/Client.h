//
// Created by Владимир on 08.09.2021.
//

#ifndef LAB1_CLIENT_H
#define LAB1_CLIENT_H


#include <string>
#include <netinet/in.h>

class Client {
private:
    int sockfd;
    bool isIPv4;
    int uuid;
    struct sockaddr_in stSockAddr;
    const int delay_secs = 1;
    const char *message = "Hello, World!";
public:
    Client(int port, std::string multicast_group);
    void run();
    ~Client();
};


#endif //LAB1_CLIENT_H
