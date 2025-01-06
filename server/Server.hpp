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
        void start();
        void loop();
        void stop();
        int getPort() const;
        void broadcastNewPackets();
        void broadcastLuaPackets();
        void sendToClients();

    private:
        int _port;
        bool _isRunning;
        ServerConnection _serverConnection;
};

#endif /* !SERVER_HPP_ */
