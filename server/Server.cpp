/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#include "Server.hpp"
#include "Engine.hpp"
#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "spdlog/spdlog.h"

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
        engine.callHook("InitServer", nullptr);
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

    engine.callHook("Tick", "int", engine.deltaTime(), nullptr);
    for (auto client : _serverConnection.getClientConnections()) {
        if (!client->shouldDisconnect()) {
            broadcastNewPackets();
            broadcastLuaPackets();
            sendToClients();
            // TODO: handle multiple incoming packets per tick for both TCP and UDP, implement with a limit of packets per tick.
            if (client->hasTcpPacketInput()) {
                Packet *packet = client->popTcpPacketInput();
                (this->*PACKET_HANDLERS.at(packet->cmd))(client, packet);
                PacketBuilder(packet).reset();
            }
            if (client->hasUdpPacketInput()) {
                Packet *packet = client->popUdpPacketInput();
                (this->*PACKET_HANDLERS.at(packet->cmd))(client, packet);
                PacketBuilder(packet).reset();
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
        for (auto client : _serverConnection.getClientConnections()) {
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

void Server::handleNonePacket(Client *client, Packet* packet)
{
}

void Server::handleConnectPacket(Client *client, Packet *inPacket)
{
    PacketBuilder readBuilder(inPacket);
    PacketBuilder builder;
    switch (client->getStep())
    {
        case Client::ConnectionStep::UNVERIFIED:
            {
                spdlog::debug("First connection packet received from client {}", client->getUuid());
                readBuilder.reset();
                builder.setCmd(PacketCmd::CONNECT).writeString(SERVER_CHALLENGE);
                Packet *packet = builder.build();
                client->addTcpPacketOutput(packet);
                client->setStep(Client::ConnectionStep::AUTH_CODE_SENT);
                // Engine::GetInstance().callHook("ClientConnecting", "string", client->getUuid().c_str(), nullptr);
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_SENT:
            {
                spdlog::debug("Second connection packet received from client {}", client->getUuid());
                std::string clientChallengeCode = readBuilder.readString();
                if (clientChallengeCode == CLIENT_CHALLENGE) {
                    builder.setCmd(PacketCmd::CONNECT).writeString("AUTHENTICATED");
                    client->addTcpPacketOutput(builder.build());
                    client->setStep(Client::ConnectionStep::AUTH_CODE_VERIFIED);
                } else {
                    client->disconnect();
                }
                readBuilder.reset();
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_VERIFIED:
            {
                spdlog::debug("Client {} is now connected", client->getUuid());
                readBuilder.reset();
                for (const auto & [packetName, reliable] : Engine::GetInstance().getPacketsRegistry()) {
                    builder.setCmd(PacketCmd::NEW_MESSAGE).writeString(packetName).writeInt(reliable);
                    client->addTcpPacketOutput(builder.build());
                }
                client->setStep(Client::ConnectionStep::COMPLETE);
                // Engine::GetInstance().callHook("ClientConnected", "string", client->getUuid().c_str(), nullptr);
                break;
            }
        default:
            break;
    }
}

void Server::handleDisconnectPacket(Client *client, Packet* packet)
{
    client->disconnect();
}

void Server::handleNewMessagePacket(Client *client, Packet* packet)
{
}

void Server::handleLuaPacket(Client *client, Packet* packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet, client->getUuid());
}

void Server::broadcastNewPackets()
{
    if (Engine::GetInstance().hasNewPacketToBroadcast())
    {
        while (Engine::GetInstance().getNewPacketsInRegistry().size() > 0) {
            const std::string packetName = Engine::GetInstance().getNewPacketsInRegistry().front();
            const bool reliable = Engine::GetInstance().isPacketReliable(packetName);
            for (auto &client : _serverConnection.getClientConnections()) {
                if (client->getStep() != Client::ConnectionStep::COMPLETE)
                    continue;
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
