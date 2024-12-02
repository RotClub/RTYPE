/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"

int main(void) {
    Client &client = Client::InitiateInstance("127.0.0.1", 5000);
    Engine &engine = Engine::GetInstance();
    engine.Log(Engine::LogLevel::INFO, "Client started");
    engine.Log(Engine::LogLevel::INFO, "IP: " + client.getIp());
    engine.Log(Engine::LogLevel::INFO, "Port: " + std::to_string(client.getPort()));
    client.getServerConnection().connectToServer();
    client.getServerConnection().establishConnection();
    return 0;
}
