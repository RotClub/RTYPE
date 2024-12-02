/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#include "GlobalConnection.hpp"

GlobalConnection::GlobalConnection()
{
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
}


int GlobalConnection::_selectFd() {
    int retval;
    timeval tv = {1, 0};

    FD_ZERO(&_readfds);
    FD_SET(_fd, &_readfds);
    retval = select(_fd + 1, &_readfds, NULL, NULL, &tv);
    if (retval == -1) {
        throw std::runtime_error("Error selecting socket");
    }
    return retval;
}
