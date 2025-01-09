/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "../common/Engine.hpp"
    #include "connection/ClientConnection.hpp"
    #include "game/Game.hpp"
    #include <string>
    #include <unordered_map>

    class Client {

        public:
            Client(Client const &) = delete;
            void operator=(Client const &) = delete;

            enum class ConnectionStep {
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
                {PacketCmd::NONE, (void (Client::*)(Packet *))nullptr},
                {PacketCmd::CONNECT, &Client::handleConnectPacket},
                {PacketCmd::DISCONNECT, &Client::handleDisconnectPacket},
                {PacketCmd::NEW_MESSAGE, &Client::handleNewMessagePacket},
                {PacketCmd::NET, &Client::handleLuaPacket},
            };

            void startGame();
            void setupLua();
            void loadLuaGame();
            std::string getIp() const;
            int getPort() const;
            ClientConnection &getClientConnectionTcp();
            ClientConnection &getClientConnectionUdp();
            ResourceManager &getResourceManager();

            void broadcastLuaPackets();
            void processIncomingPackets();

            bool isConnectionEstablished() const { return _connectionEstablished; }

        protected:
            Client(std::string ip, int port);
            static Client *_instance;

        private:
            ConnectionStep _step;
            bool _connectionEstablished;

            std::string _ip;
            int _port;
            ClientConnection _clientConnectionTcp;
            ClientConnection _clientConnectionUdp;
            Game _game;
            // loadingScreen
    };

#endif /* !CLIENT_HPP_ */
