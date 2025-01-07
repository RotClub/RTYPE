/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "Networking/PacketBuilder.hpp"

#include "spdlog/spdlog.h"
#include <cstdlib>

int main(void)
{
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    client.setupLua();
    try {
        client.getClientConnectionTcp().connectToServer();
    } catch (const std::exception &e) {
        spdlog::error(e.what());
        return 84;
    }
}
