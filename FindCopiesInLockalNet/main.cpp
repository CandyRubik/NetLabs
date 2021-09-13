#include "ClientServerIPv4.h"
#include "ClientServerIPv6.h"

#define PORT 4321

int main(int argc, char **argv) {
    std:: string groupAddr = std::string(argv[1]);
    if(std::count(groupAddr.begin(), groupAddr.end(), ':') > 1) {
        ClientServerIPv6 clientServer = ClientServerIPv6(PORT,std::string(argv[1]));
        clientServer.createSocket();
        clientServer.setUdpSocket();
        clientServer.bindUdpSocket();
        clientServer.run();
    } else {
        ClientServerIPv4 clientServer = ClientServerIPv4(PORT,std::string(argv[1]));
        clientServer.createSocket();
        clientServer.setUdpSocket();
        clientServer.bindUdpSocket();
        clientServer.run();
    }

}