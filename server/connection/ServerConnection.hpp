/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#ifndef SERVERCONNECTION_HPP_
    #define SERVERCONNECTION_HPP_

    #include "Networking/GlobalConnection.hpp"
    #include "Networking/Defines.hpp"
    #include "Networking/SafeQueue.hpp"
    #include "Networking/Packet.hpp"
    #include "Networking/PacketBuilder.hpp"
    #include <map>
    #include <thread>
    #include <set>
#include <client/Client.hpp>
#include <sys/select.h>
    #include <netinet/in.h>
    #include <sys/socket.h>

    #define KEYWORD "xMmM21B6dFdwJY39"

    class ServerConnection {
        public:
            ServerConnection(int port);
            ~ServerConnection();

            void start();
            void stop();

        private:
            void _loop();
            void _receiveLoop();
            void _sendLoop();
            Packet _tryReceive();
            void _createSocket();
            void _setClientFds(fd_set *set);
            int _getMaxFd();
            int _selectFd();

            int _port;
            std::string _ip;
            std::atomic<bool> _running = false;
            std::thread _thread;
            fd_set _readfds;
            fd_set _writefds;
            int _tcpFd = -1;
            int _udpFd = -1;
            std::vector<Client> _clientConnections;
            sockaddr_in _addr;
};

#endif /* !SERVERCONNECTION_HPP_ */
