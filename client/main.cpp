/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"

int main(void)
{
	initLittleWindow();
    Client &client = Client::InitiateInstance("127.0.0.1", 5000);
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT);
    engine.Log(Engine::LogLevel::INFO, "Client started");
    engine.Log(Engine::LogLevel::INFO, "IP: " + client.getIp());
    engine.Log(Engine::LogLevel::INFO, "Port: " + std::to_string(client.getPort()));
    client.getClientConnectionTcp().connectToServer();
    client.getClientConnectionTcp().establishConnection();
    return 0;
}
