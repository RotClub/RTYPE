/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#include "GlobalConnection.hpp"
#include "Networking/Packet.hpp"

GlobalConnection::GlobalConnection()
{
}

GlobalConnection::~GlobalConnection()
{
}

Packet* GlobalConnection::getLatestTCPPacket()
{
    return std::get<IN>(_tcpQueues).dequeue();
}

Packet* GlobalConnection::getLatestUDPPacket()
{
    return std::get<IN>(_udpQueues).dequeue();
}

int GlobalConnection::_selectFd()
{
    int retval;

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_SET(_tcpFd, &_readfds);
    FD_SET(_tcpFd, &_writefds);
    FD_SET(_udpFd, &_readfds);
    FD_SET(_udpFd, &_writefds);
    retval = select(std::max(_tcpFd, _udpFd) + 1, &_readfds, &_writefds, nullptr, nullptr);
    if (retval == -1) {
        throw std::runtime_error("Error selecting socket");
    }
    return retval;
}

void GlobalConnection::_createSocket()
{
    _tcpFd = socket(AF_INET, SOCK_STREAM, 0);
    _udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_tcpFd == -1 || _udpFd == -1) {
        throw std::runtime_error("Error creating socket");
    }
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
}
