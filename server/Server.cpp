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

Server::Server(int port) : _port(port), _isRunning(false), _serverConnection(port) {}

Server::~Server() { stop(); }

void Server::start()
{
    try {
        Engine &engine = Engine::StartInstance(Types::VMState::SERVER, "rtype");
        spdlog::info("Server starting...");
        engine.displayGameInfo();
        engine.loadLibraries();
        engine.lockLuaState();
        if (engine.LoadLuaFile("index.luau"))
            engine.execute();
        else
            throw std::runtime_error("Failed to load index.luau");
        spdlog::info("Server started!");
        engine.callHook("InitServer", nullptr);
        _serverConnection.start();
        _isRunning = true;
        while (_isRunning) {
            loop();
        }
    }
    catch (const std::exception &e) {
        spdlog::error(e.what());
    }
}

void Server::loop()
{
    Engine &engine = Engine::GetInstance();

    while (!_serverConnection.getDisconnectedClients().empty())
        Engine::GetInstance().callHook("PlayerLeave", "string", _serverConnection.getDisconnectedClients().dequeue().c_str(), nullptr);
    engine.callHook("Tick", "int", engine.deltaTime(), nullptr);
    engine.updateNode(engine.root);
    for (auto client : _serverConnection.getClientConnections()) {
        if (!client->shouldDisconnect()) {
            broadcastNewPackets();
            broadcastLuaPackets();
            sendToClients();
            if (client->hasTcpPacketInput()) {
                Packet *packet = client->popTcpPacketInput();
                if (PACKET_HANDLERS.find(packet->cmd) != PACKET_HANDLERS.end())
                    (this->*PACKET_HANDLERS.at(packet->cmd))(client, packet);
                PacketBuilder(packet).reset();
            }
            if (client->hasUdpPacketInput()) {
                Packet *packet = client->popUdpPacketInput();
                if (PACKET_HANDLERS.find(packet->cmd) != PACKET_HANDLERS.end())
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
            if (client->getStep() != Client::ConnectionStep::COMPLETE)
                continue;
            if (Engine::GetInstance().isPacketReliable(newPacket.first)) {
                client->addTcpPacketOutput(newPacket.second);
            }
            else {
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
                    }
                    else {
                        client->addUdpPacketOutput(newPacket.second);
                    }
                    packetClient.second.pop();
                }
            }
        }
    }
}

void Server::handleNonePacket(Client *client, Packet *packet) {}

static std::string idToString(char *id)
{
    std::string str;
    for (int i = 0; i < 16; i++) {
        str += id[i];
    }
    return str;
}

void Server::handleConnectPacket(Client *client, Packet *inPacket)
{
    PacketBuilder readBuilder(inPacket);
    switch (client->getStep()) {
        case Client::ConnectionStep::UNVERIFIED:
            {
                readBuilder.reset();
                Packet *packet = PacketBuilder().setCmd(PacketCmd::CONNECT).writeString(SERVER_CHALLENGE).build();
                client->addTcpPacketOutput(packet);
                client->setStep(Client::ConnectionStep::AUTH_CODE_SENT);
                Engine::GetInstance().callHook("ClientConnecting", "string", client->getUuid().c_str(), nullptr);
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_SENT:
            {
                std::string clientChallengeCode = readBuilder.readString();
                if (clientChallengeCode == CLIENT_CHALLENGE) {
                    for (const auto &[packetName, reliable] : Engine::GetInstance().getPacketsRegistry()) {
                        client->addTcpPacketOutput(PacketBuilder().setCmd(PacketCmd::NEW_MESSAGE).writeString(packetName).writeInt(reliable).build());
                    }
                    client->addTcpPacketOutput(PacketBuilder().setCmd(PacketCmd::CONNECT).writeString("AUTHENTICATED").writeString(idToString(client->getID())).build());
                    client->setStep(Client::ConnectionStep::AUTH_CODE_VERIFIED);
                }
                else {
                    client->disconnect();
                }
                readBuilder.reset();
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_VERIFIED:
            {
                readBuilder.reset();
                client->setStep(Client::ConnectionStep::COMPLETE);
                spdlog::info("Client {} connected", client->getUuid());
                Engine::GetInstance().callHook("PlayerJoin", "string", client->getUuid().c_str(), nullptr);
                break;
            }
        default:
            break;
    }
}

void Server::handleDisconnectPacket(Client *client, Packet *packet) { client->disconnect(); }

void Server::handleNewMessagePacket(Client *client, Packet *packet) {}

void Server::handleLuaPacket(Client *client, Packet *packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet, client->getUuid());
}

void Server::broadcastNewPackets()
{
    if (Engine::GetInstance().hasNewPacketToBroadcast()) {
        while (Engine::GetInstance().getNewPacketsInRegistry().size() > 0) {
            const std::string packetName = Engine::GetInstance().getNewPacketsInRegistry().front();
            const bool reliable = Engine::GetInstance().isPacketReliable(packetName);
            for (auto &client : _serverConnection.getClientConnections()) {
                if (client->getStep() != Client::ConnectionStep::COMPLETE)
                    continue;
                PacketBuilder builder;
                Packet *packet =
                    builder.setCmd(PacketCmd::NEW_MESSAGE).writeString(packetName).writeInt(reliable).build();
                client->addTcpPacketOutput(packet);
            }
            Engine::GetInstance().getNewPacketsInRegistry().pop();
        }
    }
}

void Server::stop()
{
    spdlog::info("Stopping server...");
    _serverConnection.stop();
    _isRunning = false;
}

int Server::getPort() const { return _port; }
