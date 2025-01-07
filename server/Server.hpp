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
#include "Engine.hpp"

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

        void handleNonePacket(Client *client, Packet *packet);
        void handleConnectPacket(Client *client, Packet *packet);
        void handleDisconnectPacket(Client *client, Packet *packet);
        void handleNewMessagePacket(Client *client, Packet *packet);
        void handleLuaPacket(Client *client, Packet *packet);

        const std::unordered_map<PacketCmd, void (Server::*)(Client *, Packet *)> PACKET_HANDLERS = {
            {PacketCmd::NONE, &Server::handleNonePacket},
            {PacketCmd::CONNECT, &Server::handleConnectPacket},
            {PacketCmd::DISCONNECT, &Server::handleDisconnectPacket},
            {PacketCmd::NEW_MESSAGE, &Server::handleNewMessagePacket},
            {PacketCmd::NET, &Server::handleLuaPacket},
        };

    private:
        int _port;
        bool _isRunning;
        ServerConnection _serverConnection;
};

#endif /* !SERVER_HPP_ */
