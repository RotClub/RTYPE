/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <unordered_map>
#include "../common/Engine.hpp"
#include "connection/ClientConnection.hpp"
#include "game/Game.hpp"

class Client
{

    public:
        Client(Client const &) = delete;
        void operator=(Client const &) = delete;

        enum class ConnectionStep
        {
            AUTH_CODE_RECEIVED,
            AUTH_CODE_SENT,
            COMPLETE
        };

        static Client &InitiateInstance(std::string ip, int port);
        static Client &GetInstance();

        void handleConnectPacket(Packet *packet);
        void handleDisconnectPacket(Packet *packet);
        void handleLuaPacket(Packet *packet);
        void handleNewMessagePacket(Packet *packet);

        const std::unordered_map<PacketCmd, void (Client::*)(Packet *)> PACKET_HANDLERS = {
            {PacketCmd::NONE, (void(Client::*)(Packet *)) nullptr},
            {PacketCmd::CONNECT, &Client::handleConnectPacket},
            {PacketCmd::DISCONNECT, &Client::handleDisconnectPacket},
            {PacketCmd::NEW_MESSAGE, &Client::handleNewMessagePacket},
            {PacketCmd::NET, &Client::handleLuaPacket},
        };

        void startGame();
        void setupLua();
        void setupClientSideLua();
        void loadLuaGame();
        std::string getIp() const;
        int getPort() const;
        ClientConnection &getClientConnection();
        ResourceManager &getResourceManager();

        raylib::Window &getWindow() { return _game.getWindow(); }
        const Types::Vector2 &getGameSize() const;

        void broadcastLuaPackets();
        void processIncomingPackets();

        bool isConnectionEstablished() const { return _connectionEstablished; }
        void disconnectFromServer();

    protected:
        Client(std::string ip, int port);
        static Client *_instance;

    private:
        ConnectionStep _step;
        bool _connectionEstablished;

        std::string _ip;
        int _port;
        ClientConnection _clientConnection;
        Game _game;
        // loadingScreen
};

#endif /* !CLIENT_HPP_ */
