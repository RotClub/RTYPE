/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"
#include <cstdlib>
#include <lua/ClientSideLua.hpp>
#include <string>
#include "Engine.hpp"
#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "game/Game.hpp"
#include "spdlog/spdlog.h"

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

Client::Client(std::string ip, int port) :
    _ip(ip), _port(port), _clientConnection(ip, port),
    _step(ConnectionStep::AUTH_CODE_RECEIVED), _connectionEstablished(false)
{
}

void Client::startGame() { _game.run(); }

void Client::setupLua()
{
    Engine &engine = Engine::GetInstance();
    engine.loadLibraries();
    setupClientSideLua();
}

void Client::setupClientSideLua()
{
    lua_State *L = Engine::GetInstance().getLuaState();
    luau_ExposeGlobalFunction(L, luau_IsKeyPressed, "IsKeyPressed");
    luau_ExposeGlobalFunction(L, luau_IsKeyJustPressed, "IsKeyJustPressed");
    luau_ExposeGlobalFunction(L, luau_IsKeyReleased, "IsKeyReleased");
    luau_ExposeGlobalFunction(L, luau_EnableFpsCounter, "EnableFpsCounter");
    luau_ExposeGlobalFunction(L, luau_DisableFpsCounter, "DisableFpsCounter");
    luau_ExposeGlobalFunction(L, luau_DisableColorBlindnessShader, "DisableColorBlindnessShader");
    luau_ExposeGlobalFunction(L, luau_CycleColorBlindnessShader, "CycleColorBlindnessShader");
    luau_ExposeGlobalFunction(L, luau_WindowHeight, "GetWindowHeight");
    luau_ExposeGlobalFunction(L, luau_WindowWidth, "GetWindowWidth");
}

const Types::Vector2 &Client::getGameSize() const
{
    return _game.getWindowSize();
}


void Client::loadLuaGame()
{
    Engine &engine = Engine::GetInstance();
    engine.displayGameInfo();
    engine.lockLuaState();
    if (engine.LoadLuaFile("index.luau"))
        engine.execute();
    engine.callHook("InitClient", nullptr);
}

std::string Client::getIp() const { return _ip; }

int Client::getPort() const { return _port; }

ClientConnection &Client::getClientConnection() { return _clientConnection; }

void Client::disconnectFromServer()
{
    spdlog::info("Disconnecting from server");
    _clientConnection.disconnectFromServer();
    _connectionEstablished = false;
}

void Client::broadcastLuaPackets()
{
    while (!Engine::GetInstance().getBroadcastQueue().empty()) {
        std::pair<std::string, Packet *> newPacket = Engine::GetInstance().getBroadcastQueue().front();
        if (Engine::GetInstance().isPacketReliable(newPacket.first)) {
            getClientConnection().sendToServerTCP(newPacket.second);
        }
        else {
            getClientConnection().sendToServerUDP(newPacket.second);
        }
        Engine::GetInstance().getBroadcastQueue().pop();
    }
}

void Client::processIncomingPackets()
{
    while (getClientConnection().hasPendingTCPPacket()) {
        Packet *packet = getClientConnection().getLatestTCPPacket();
        if (packet == nullptr)
            return;
        if (packet->cmd != PacketCmd::NONE)
            (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
        PacketBuilder(packet).reset();
        delete packet;
    }
    while (getClientConnection().hasPendingUDPPacket()) {
        Packet *packet = getClientConnection().getLatestUDPPacket();
        if (packet == nullptr)
            return;
        if (packet->cmd != PacketCmd::NONE)
            (this->*PACKET_HANDLERS.at(packet->cmd))(packet);
        PacketBuilder(packet).reset();
        delete packet;
    }
}

void Client::handleConnectPacket(Packet *packet)
{
    PacketBuilder readBuilder(packet);
    switch (_step) {
        case Client::ConnectionStep::AUTH_CODE_RECEIVED:
            {
                std::string authCode = readBuilder.readString();
                if (authCode == SERVER_CHALLENGE) {
                    getClientConnection().sendToServerTCP(PacketBuilder().setCmd(PacketCmd::CONNECT).writeString(CLIENT_CHALLENGE).build());
                    _step = Client::ConnectionStep::AUTH_CODE_SENT;
                }
                else {
                    throw std::runtime_error("Invalid auth code");
                }
                break;
            }
        case Client::ConnectionStep::AUTH_CODE_SENT:
            {
                std::string message = readBuilder.readString();
                if (message == "AUTHENTICATED") {
                    std::string id = readBuilder.readString();
                    std::string selectedGame = readBuilder.readString();
                    Client::GetInstance().getWindow().SetTitle(selectedGame);
                    getClientConnection().setID(id);
                    Engine::GetInstance().loadGame(selectedGame);
                    getClientConnection().sendToServerTCP(PacketBuilder().setCmd(PacketCmd::CONNECT).build());
                    getClientConnection().sendToServerUDP(PacketBuilder().setCmd(PacketCmd::NONE).build());
                    _step = Client::ConnectionStep::COMPLETE;
                }
                else {
                    throw std::runtime_error("Failed to authenticate");
                }
                readBuilder.reset();
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

void Client::handleDisconnectPacket(Packet *packet) { spdlog::debug("Disconnected from server"); }

void Client::handleLuaPacket(Packet *packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    Engine::GetInstance().netCallback(packetName, packet, packetName);
}

void Client::handleNewMessagePacket(Packet *packet)
{
    PacketBuilder builder(packet);
    std::string packetName = builder.readString();
    bool reliable = builder.readBool();
    spdlog::debug("Packet name: {}, reliable: {}", packetName, reliable);
    Engine::GetInstance().addPacketRegistryEntry(packetName, reliable);
}
