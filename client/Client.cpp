/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"
#include "Engine.hpp"
#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "game/Game.hpp"
#include "spdlog/spdlog.h"
#include <cstdlib>
#include <string>

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
    _clientConnectionTcp(ip, port, false), _clientConnectionUdp(ip, port, true), _step(ConnectionStep::AUTH_CODE_RECEIVED)
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
    engine.callHook("InitClient", nullptr);
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
    while (getClientConnectionTcp().hasPendingPacket()) {
        Packet *packet = getClientConnectionTcp().getLatestPacket();
        if (packet == nullptr)
            return;
        (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
        std::free(packet->data);
        delete packet;
    }
    while (getClientConnectionUdp().hasPendingPacket()) {
        Packet *packet = getClientConnectionUdp().getLatestPacket();
        if (packet == nullptr)
            return;
        (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
        std::free(packet->data);
        delete packet;
    }
}

void Client::handleConnectPacket(Packet *packet)
{
    spdlog::debug("Handling connect packet");
    PacketBuilder readBuilder(packet);
    PacketBuilder builder;
    switch (_step)
    {
        case Client::ConnectionStep::AUTH_CODE_RECEIVED:
            {
                spdlog::debug("Received auth code from server");
                std::string authCode = readBuilder.readString();
                spdlog::debug("Auth code: {}", authCode);
                if (authCode == SERVER_CHALLENGE) {
                    builder.setCmd(PacketCmd::CONNECT).writeString(CLIENT_CHALLENGE);
                    Packet *packet = builder.build();
                    getClientConnectionTcp().sendToServer(packet);
                    spdlog::debug("Sent auth code verification to server: {}", packet->n);
                    _step = Client::ConnectionStep::AUTH_CODE_SENT;
                } else {
                    throw std::runtime_error("Invalid auth code");
                }
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_SENT:
            {
                spdlog::debug("Received auth code verification from server");
                std::string message = readBuilder.readString();
                readBuilder.reset();
                spdlog::debug("Auth code verification: {}", message);
                if (message == "AUTHENTICATED") {
                    builder.setCmd(PacketCmd::CONNECT);
                    getClientConnectionTcp().sendToServer(builder.build());
                    _step = Client::ConnectionStep::COMPLETE;
                } else {
                    throw std::runtime_error("Failed to authenticate");
                }
                spdlog::debug("Connection established");
                _connectionEstablished = true;
                break;
            }
        case Client::ConnectionStep::COMPLETE:
            {
                break;
            }
        default:
            break;
    }
}

void Client::handleDisconnectPacket(Packet *packet)
{
    spdlog::debug("Disconnected from server");
}

void Client::handleLuaPacket(Packet *packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet, packetName);
}

void Client::handleNewMessagePacket(Packet *packet)
{
    spdlog::debug("Received new message packet");
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    int reliable = builder.readInt();
    spdlog::debug("Packet name: {}, reliable: {}", packetName, reliable);
    Engine::GetInstance().addPacketRegistryEntry(packetName, static_cast<bool>(reliable));
}
