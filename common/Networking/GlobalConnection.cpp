/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#include "GlobalConnection.hpp"

GlobalConnection::GlobalConnection(bool udp)
    : _udp(udp)
{
    _createSocket();
    _queues = std::make_tuple(new SafeQueue<Packet>(), new SafeQueue<Packet>());
}

GlobalConnection::~GlobalConnection()
{
    delete std::get<IN>(_queues);
    delete std::get<OUT>(_queues);
}

Packet GlobalConnection::getLatestPacket()
{
    Packet pckt = std::get<IN>(_queues)->dequeue();
    if (pckt.size == 0)
        return NULL_PACKET;
    return pckt;
}

bool GlobalConnection::isUdp() const
{
    return _udp;
}

int GlobalConnection::_selectFd()
{
    int retval;
    timeval tv = {1, 0};

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_SET(_fd, &_readfds);
    FD_SET(_fd, &_writefds);
    retval = select(_fd + 1, &_readfds, &_writefds, NULL, &tv);
    if (retval == -1) {
        throw std::runtime_error("Error selecting socket");
    }
    return retval;
}

void GlobalConnection::_createSocket()
{
    if (_udp)
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
    else
        _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        throw std::runtime_error("Error creating socket");
    }
    _addr.sin_family = AF_INET;
}
