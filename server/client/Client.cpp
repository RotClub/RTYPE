//
// Created by Théo Bary on 10/12/2024.
//

#include "Client.hpp"
#include <iostream>

Client::Client(const int fd, const sockaddr_in address)
    : _tcpFd(fd), _address(address)
{
    memset(&_address, 0, sizeof(_address));
}

Client::~Client()
{
}
