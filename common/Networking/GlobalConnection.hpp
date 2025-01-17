/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#ifndef GLOBALCONNECTION_HPP_
#define GLOBALCONNECTION_HPP_

#include "Defines.hpp"
#include "Packet.hpp"
#include "SafeQueue.hpp"

#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <thread>
#include <tuple>
#include <unistd.h>

class GlobalConnection
{
    public:
        virtual ~GlobalConnection();

        virtual Packet *getLatestTCPPacket();
        virtual Packet *getLatestUDPPacket();

    protected:
        GlobalConnection();

        virtual int _selectFd();
        virtual void _createSocket();
        virtual void _loop() = 0;

        Packet *_packet;
        std::tuple<SafeQueue<Packet *>, SafeQueue<Packet *>> _tcpQueues;
        std::tuple<SafeQueue<Packet *>, SafeQueue<Packet *>> _udpQueues;
        std::thread _thread;
        int _tcpFd = -1;
        int _udpFd = -1;
        fd_set _readfds;
        fd_set _writefds;
        sockaddr_in _addr;
};

#endif /* !GLOBALCONNECTION_HPP_ */
