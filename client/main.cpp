/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "ArgumentManager/ClientArgumentManager.hpp"

#include <cstdlib>


int main(int argc, char **argv)
{
    ClientArgumentManager argManager(argc, argv);
    if (!argManager.checkClientArguments())
        return 84;
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    client.getClientConnectionTcp().connectToServer();
    client.startGame();
}
