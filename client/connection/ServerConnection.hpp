/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#ifndef SERVERCONNECTION_HPP_
    #define SERVERCONNECTION_HPP_

    #include "Defines.hpp"
    #include "connection/SafeQueue.hpp"

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
    #include <sys/time.h>
    #include <sys/types.h>
    #include <algorithm>

    class ServerConnection {
    public:
        ServerConnection(std::string ip, int port, std::tuple<SafeQueue<std::string> *, SafeQueue<std::string> *> queues);
        ~ServerConnection();

        void connectToServer();
        void disconnectFromServer();
        std::string tryReceive();
        void sendToServer(std::string msg, std::string endWith = "\n");


    private:
        void _serverLoop();
        int _selectFd();
        void _receiveLoop();
        void _sendLoop();

        std::string _ip;
        int _port;
        std::atomic<bool> _connected = false;
        std::string _buffer;
        std::tuple<SafeQueue<std::string> *, SafeQueue<std::string> *> _queues;
        std::thread _thread;
        int _fd = -1;
        fd_set _readfds;
        struct sockaddr_in _addr;
};

#endif /* !SERVERCONNECTION_HPP_ */
