
#ifndef LAB1_SERVER_H
#define LAB1_SERVER_H
#include <string>
#include <netinet/in.h>

class Server {
private:
    int sockfd;
    bool isIPv4;
    int uuid;
    struct sockaddr_in stSockAddr;
public:
    Server(int port, std::string multicast_group);
    void setUUID(int id);
    void run();
    ~Server();

};


#endif //LAB1_SERVER_H
