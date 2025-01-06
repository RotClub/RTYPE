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

    class Client {
        public:
            Client(Client const &) = delete;
            void operator=(Client const &) = delete;

            static Client &InitiateInstance(std::string ip, int port);
            static Client &GetInstance();

            static void handleConnectPacket(Packet *packet);
            static void handleDisconnectPacket(Packet *packet);
            static void handleLuaPacket(Packet *packet);
            static void handleNewMessagePacket(Packet *packet);

            std::map<PacketCmd, std::function<void(Packet *)>> packetHandlers = {
                {PacketCmd::CONNECT, handleConnectPacket},
                {PacketCmd::DISCONNECT, handleDisconnectPacket},
                {PacketCmd::NEW_MESSAGE, handleNewMessagePacket},
                {PacketCmd::NET, handleLuaPacket},
            };

            void startGame();
            std::string getIp() const;
            int getPort() const;
            ClientConnection &getClientConnectionTcp();
            ClientConnection &getClientConnectionUdp();
            ResourceManager &getResourceManager();

            void broadcastLuaPackets();
            void processIncomingPackets();

        protected:
            Client(std::string ip, int port);
            static Client *_instance;

        private:
            std::string _ip;
            int _port;
            ClientConnection _clientConnectionTcp;
            ClientConnection _clientConnectionUdp;
            Game _game;
            // loadingScreen
    };

#endif /* !CLIENT_HPP_ */
