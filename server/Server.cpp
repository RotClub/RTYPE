/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(int port)
    : _port(port), _isRunning(false), _serverConnection(port)
{
}

Server::~Server()
{
    stop();
}

void Server::start()
{
    try {
        Engine &engine = Engine::StartInstance(Types::VMState::SERVER, "rtype");
        spdlog::info("Server starting...");
        engine.displayGameInfo();
        engine.loadLibraries();
        if (engine.LoadLuaFile("index.luau"))
            engine.execute();
        spdlog::info("Server started!");
        engine.callHook("RType:InitServer", nullptr);
        _serverConnection.start();
        _isRunning = true;
        while (_isRunning) {
            loop();
        }
    } catch (const std::exception &e) {
        spdlog::error(e.what());
    }
}

void Server::loop()
{
    Engine &engine = Engine::GetInstance();

    engine.callHook("RType:Tick", "int", engine.deltaTime(), nullptr);
    for (auto &client : _serverConnection.getClientConnections()) {
        if (!client->shouldDisconnect()) {
            broadcastNewPackets();
            broadcastLuaPackets();
            sendToClients();
            // TODO: handle multiple incoming packets per tick for both TCP and UDP, implement with a limit of packets per tick.
            if (client->hasTcpPacketInput()) {
                Packet *packet = client->popTcpPacketInput();
                (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
            }
            if (client->hasUdpPacketInput()) {
                Packet *packet = client->popUdpPacketInput();
                (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
            }
        }
    }
}

void Server::broadcastLuaPackets()
{
    while (!Engine::GetInstance().getBroadcastQueue().empty()) {
        std::pair<std::string, Packet *> newPacket = Engine::GetInstance().getBroadcastQueue().front();
        for (auto &client : _serverConnection.getClientConnections()) {
            if (Engine::GetInstance().isPacketReliable(newPacket.first)) {
                client->addTcpPacketOutput(newPacket.second);
            } else {
                client->addUdpPacketOutput(newPacket.second);
            }
        }
        Engine::GetInstance().getBroadcastQueue().pop();
    }
}

void Server::sendToClients()
{
    for (auto &packetClient : Engine::GetInstance().getSendToClientMap()) {
        for (auto &client : _serverConnection.getClientConnections()) {
            if (client->getUuid() == packetClient.first) {
                while (!packetClient.second.empty()) {
                    std::pair<std::string, Packet *> newPacket = packetClient.second.front();
                    if (Engine::GetInstance().isPacketReliable(newPacket.first)) {
                        client->addTcpPacketOutput(newPacket.second);
                    } else {
                        client->addUdpPacketOutput(newPacket.second);
                    }
                    packetClient.second.pop();
                }
            }
        }
    }
}

void Server::handleNonePacket(Packet* packet)
{
}

void Server::handleConnectPacket(Packet* packet)
{
}

void Server::handleDisconnectPacket(Packet* packet)
{
}

void Server::handleNewMessagePacket(Packet* packet)
{
}

void Server::handleLuaPacket(Packet* packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet);
}

void Server::broadcastNewPackets()
{
    if (Engine::GetInstance().hasNewPacketToBroadcast())
    {
        while (Engine::GetInstance().getNewPacketsInRegistry().size() > 0) {
            const std::string packetName = Engine::GetInstance().getNewPacketsInRegistry().front();
            const bool reliable = Engine::GetInstance().isPacketReliable(packetName);
            for (auto &client : _serverConnection.getClientConnections()) {
                PacketBuilder builder;
                Packet *packet = builder.setCmd(PacketCmd::NEW_MESSAGE).writeString(packetName).writeInt(reliable).build();
                client->addTcpPacketOutput(packet);
            }
            Engine::GetInstance().getNewPacketsInRegistry().pop();
        }
    }
}

void Server::stop()
{
    _serverConnection.stop();
    _isRunning = false;
}

int Server::getPort() const
{
    return _port;
}
