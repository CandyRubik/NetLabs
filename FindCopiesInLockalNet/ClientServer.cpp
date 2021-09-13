//
// Created by rubik on 10.09.2021.
//

#include <uuid.h>
#include <unistd.h>
#include "ClientServer.h"

ClientServer::ClientServer(int port, std::string groupAddr) {
    this->port = port;
    this->groupAddr = groupAddr;
    uuid_t id;
    uuid_generate(id);
    uuid_unparse(id, this->uuid);
    clients = { {std::string(this->uuid), -1} };
}

ClientServer::~ClientServer() {
    close(socketFD);
}
