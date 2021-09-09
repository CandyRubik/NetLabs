#include "ClientServer.h"

#define PORT 4321

int main(int argc, char **argv) {
    ClientServer clientServer = ClientServer(PORT,std::string(argv[1]), std::string(argv[2]))
                                .createSocket()
                                .setUdpSocket()
                                .bindUdpSocket();
    clientServer.run();
}