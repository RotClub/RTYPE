/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "ArgumentManager/ArgumentManager.hpp"
#include "Client.hpp"

#include <cstdlib>


int main(int argc, char **argv)
{
    ArgumentManager manager(argc, argv);
    manager.addRequiredArgument("ip");
    manager.addRequiredArgument("port");
    manager.parseArguments();

    if (!manager.checkClientArguments())
        ArgumentManager::DisplayClientUsage();

    int port = static_cast<int>(std::strtol(manager.getArgument("port").c_str(), nullptr, 10));
    Engine::StartInstance(Types::VMState::CLIENT, "");
    Client &client = Client::InitiateInstance(manager.getArgument("ip"), port);
    client.getClientConnection().connectToServer();
    client.startGame();
}
