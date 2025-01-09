/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include <cstdlib>

int main(void)
{
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    client.getClientConnectionTcp().connectToServer();
    client.startGame();
}
