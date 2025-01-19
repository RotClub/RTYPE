/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#include "GlobalConnection.hpp"
#include "Networking/Packet.hpp"

Packet *GlobalConnection::getLatestTCPPacket() { return std::get<PACKET_IN>(_tcpQueues).dequeue(); }

Packet *GlobalConnection::getLatestUDPPacket() { return std::get<PACKET_IN>(_udpQueues).dequeue(); }

GlobalConnection::GlobalConnection()
{
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock.\n");
    }
#endif
}

GlobalConnection::~GlobalConnection()
{
#ifdef WIN32
    WSACleanup();
#endif
}

int GlobalConnection::_selectFd()
{
    int retval;
    timeval timeout = {2, 0};

#ifndef far
#define far
#endif

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_SET(_tcpFd, &_readfds);
    FD_SET(_tcpFd, &_writefds);
    FD_SET(_udpFd, &_readfds);
    FD_SET(_udpFd, &_writefds);

#ifdef WIN32
    retval = select(0, &_readfds, &_writefds, nullptr, &timeout);
#else
    retval = select(std::max(_tcpFd, _udpFd) + 1, &_readfds, &_writefds, nullptr, &timeout);
#endif

    if (retval == SOCKET_ERROR) {
        throw std::runtime_error("Error selecting socket: " + std::to_string(
            #ifdef WIN32
                WSAGetLastError()
            #else
                errno
            #endif
        ));
    }
    return retval;
}

void GlobalConnection::_createSocket()
{
    _tcpFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _udpFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (_udpFd == -1 || _tcpFd == -1) {
        throw std::runtime_error("Error creating sockets");
    }

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
}
