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
	char *ip = argv[3];
	int port = static_cast<int>(std::strtol(argv[4], nullptr, 10));
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance(ip, port);
    client.getClientConnectionTcp().connectToServer();
    client.startGame();
}
