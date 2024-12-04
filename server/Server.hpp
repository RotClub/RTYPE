/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class Server {
    public:
        Server(int port);
        bool start();
        void stop();
        void handleTCPClient(int clientSocket);
        void handleUDP();

    protected:
    private:
        int _port;
        int _tcpSocket;
        int _udpSocket;
        bool _isRunning;
        sockaddr_in serverAddr;
        std::vector<std::thread> clientThreads;
};

#endif /* !SERVER_HPP_ */
