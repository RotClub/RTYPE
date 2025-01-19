/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "ArgumentManager/ArgumentManager.hpp"
#include "Client.hpp"
#include "Engine.hpp"
#include "config/Config.hpp"

#include <cstdlib>
#include <string>


int main(int argc, char **argv)
{
    ArgumentManager manager(argc, argv);
    manager.addRequiredArgument("ip");
    manager.addDefaultArgument("port", "25777");
    manager.addDefaultArgument("colorblindness", "none");
    manager.parseArguments();

    if (!manager.checkClientArguments()) {
        ArgumentManager::DisplayClientUsage();
        throw std::runtime_error("Invalid arguments");
    }

    std::string colorBlindness = manager.getArgument("colorblindness");
    if (colorBlindness == "protanopia")
        Config::GetInstance().setColorBlindnessMode(Config::ColorBlindnessMode::PROTANOPIA);
    else if (colorBlindness == "deuteranopia")
        Config::GetInstance().setColorBlindnessMode(Config::ColorBlindnessMode::DEUTERANOPIA);
    else if (colorBlindness == "tritanopia")
        Config::GetInstance().setColorBlindnessMode(Config::ColorBlindnessMode::TRITANOPIA);

    int port = static_cast<int>(std::strtol(manager.getArgument("port").c_str(), nullptr, 10));
    Engine::StartInstance(Types::VMState::CLIENT, "");
    Client &client = Client::InitiateInstance(manager.getArgument("ip"), port);
    client.getClientConnection().connectToServer();
    client.startGame();
}
