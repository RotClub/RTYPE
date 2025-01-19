/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#ifndef SERVERCONNECTION_HPP_
#define SERVERCONNECTION_HPP_

#include <client/Client.hpp>
#include <map>

#ifdef WIN32
    #include <WindowsCross.hpp>
    #include <winsock2.h>
    #include <io.h>
#else
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include <set>
#include <thread>
#include "Networking/Defines.hpp"
#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "Networking/SafeQueue.hpp"

#define CLIENT_KEYWORD "xMmM21B6dFdwJY39"
#define SERVER_KEYWORD "hXhHebiL57op0F1C"

class ServerConnection
{
    public:
        ServerConnection(int port);
        ~ServerConnection();

        std::vector<Client *> &getClientConnections() { return _clientConnections; }
        SafeQueue<std::string> &getDisconnectedClients() { return _disconnectedClients; }
        std::mutex &getClientsMutex() { return _clientsMutex; }

        void start();
        void stop();

    private:
        void _loop();
        void _receiveLoop();
        void _sendLoop();
        void _accept();
        void _disconnectClients();
        void _tryReceiveTCP(Client *client);
        void _tryReceiveUDP();
        void _processTcpBuffer(Client *client);
        void _processUdpBuffer(const std::pair<std::string, int>& addr, sockaddr_in *addr_in);
        void _createSocket();
        void _setClientFds(fd_set *set);
        int _getMaxFd();
        int _selectFd();
        Client *_getClientByID(const char id[16]);

        int _port;
        std::atomic<bool> _running = false;
        std::thread _networkThread;
        fd_set _readfds;
        fd_set _writefds;
        int _tcpFd = -1;
        int _udpFd = -1;
        std::mutex _clientsMutex;
        std::vector<Client *> _clientConnections;
        SafeQueue<std::string> _disconnectedClients;
        sockaddr_in _addr;
        std::map<std::pair<std::string, int>, std::vector<uint8_t>> _udpBuffers;
};

#endif /* !SERVERCONNECTION_HPP_ */
