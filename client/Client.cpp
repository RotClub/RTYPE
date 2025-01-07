/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"
#include "game/Game.hpp"
#include <cstdlib>

Client *Client::_instance = nullptr;

Client &Client::InitiateInstance(std::string ip, int port)
{
    if (_instance != nullptr)
        return Client::GetInstance();
    _instance = new Client(ip, port);
    return *_instance;
}

Client &Client::GetInstance()
{
    if (_instance == nullptr)
        throw std::runtime_error("Instance not initiated");
    return *_instance;
}

Client::Client(std::string ip, int port)
    : _ip(ip), _port(port),
    _clientConnectionTcp(ip, port, false), _clientConnectionUdp(ip, port, true)
{
}

void Client::startGame()
{
    _game.run();
}

void Client::setupLua()
{
    Engine &engine = Engine::GetInstance();
    engine.displayGameInfo();
    engine.loadLibraries();
    if (engine.LoadLuaFile("index.luau"))
        engine.execute();
    engine.callHook("RType:InitClient", nullptr);
}

std::string Client::getIp() const
{
    return _ip;
}

int Client::getPort() const {
    return _port;
}

ClientConnection &Client::getClientConnectionTcp()
{
    return _clientConnectionTcp;
}

ClientConnection &Client::getClientConnectionUdp()
{
    return _clientConnectionUdp;
}

void Client::broadcastLuaPackets()
{
    while (!Engine::GetInstance().getBroadcastQueue().empty()) {
        std::pair<std::string, Packet *> newPacket = Engine::GetInstance().getBroadcastQueue().front();
            if (Engine::GetInstance().isPacketReliable(newPacket.first)) {
                getClientConnectionTcp().sendToServer(newPacket.second);
            } else {
                getClientConnectionUdp().sendToServer(newPacket.second);
            }
        Engine::GetInstance().getBroadcastQueue().pop();
    }
}

void Client::processIncomingPackets()
{
    Client &client = Client::GetInstance();
    while (client.getClientConnectionTcp().hasPendingPacket()) {
        Packet *packet = client.getClientConnectionTcp().getLatestPacket();
        if (packet == nullptr)
            return;
        client.packetHandlers[packet->cmd](packet);
        free(packet->data);
        free(packet);
    }
    while (client.getClientConnectionUdp().hasPendingPacket()) {
        Packet *packet = client.getClientConnectionUdp().getLatestPacket();
        if (packet == nullptr)
            return;
        client.packetHandlers[packet->cmd](packet);
        free(packet->data);
        free(packet);
    }
}

void Client::handleConnectPacket(Packet *packet)
{
    spdlog::info("Connected to server");
}

void Client::handleDisconnectPacket(Packet *packet)
{
    spdlog::info("Disconnected from server");
}

void Client::handleLuaPacket(Packet *packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet);
}

void Client::handleNewMessagePacket(Packet *packet)
{
    spdlog::info("Received new message packet");
}
