//
// Created by Théo Bary on 10/12/2024.
//

#include "Client.hpp"
#include <iostream>
#include <cstring>

Client::Client(const int fd, const sockaddr_in address)
    : _tcpFd(fd), _address(address)
{
    std::memset(&_address, 0, sizeof(_address));
}

Client::~Client()
{
}
