/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#include "Server.hpp"

#include <Engine.hpp>

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
        spdlog::error("Error starting server: " + std::string(e.what()));
        throw;
    }
}

void Server::loop()
{
    Engine &engine = Engine::GetInstance();
    engine.callHook("RType:Tick", "int", engine.deltaTime(), nullptr);

    for (auto &client : _serverConnection.getClientConnections()) {
        if (client->hasTcpPacketInput()) {
            Packet *packet = client->popTcpPacketInput();
            PacketBuilder builder(packet);
            spdlog::info(builder.readInt());
            spdlog::info(builder.readInt());
            spdlog::info(builder.readString());
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
