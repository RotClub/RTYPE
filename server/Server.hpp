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
#include "client/Client.hpp"
#include <vector>

class Server {
    public:
        Server(int port);
        ~Server();
        bool start();
        void stop();
        int getPort() const;

    private:
        int _port;
        bool _isRunning;
        ServerConnection _serverConnection;
        std::vector<Client> _clientConnections;
};

#endif /* !SERVER_HPP_ */
