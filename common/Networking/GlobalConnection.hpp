/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** GlobalConnection
*/

#ifndef GLOBALCONNECTION_HPP_
    #define GLOBALCONNECTION_HPP_

    #include "Packet.hpp"
    #include "Defines.hpp"
    #include "SafeQueue.hpp"

    #include <string>
    #include <thread>
    #include <atomic>
    #include <tuple>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <iostream>
    #include <cstring>
    #include <sys/select.h>

    class GlobalConnection {
        public:
            virtual ~GlobalConnection();

            virtual Packet getLatestPacket();
            virtual bool isUdp() const;

        protected:
            GlobalConnection(bool udp = false);

            virtual int _selectFd();
            virtual void _createSocket();
            virtual void _loop() = 0;

            Packet _packet;
            std::tuple<SafeQueue<Packet> *, SafeQueue<Packet> *> _queues;
            std::thread _thread;
            int _fd = -1;
            fd_set _readfds;
            fd_set _writefds;
            struct sockaddr_in _addr;
            bool _udp;
    };

#endif /* !GLOBALCONNECTION_HPP_ */
