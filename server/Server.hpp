/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include "connection/ServerConnection.hpp"

class Server {
    public:
        Server(int port);
        bool start();
        void stop();
        int getPort() const;

    protected:
    private:
        int _port;
        bool _isRunning;
        ServerConnection _serverConnectionTcp;
        ServerConnection _serverConnectionUdp;
};

#endif /* !SERVER_HPP_ */
