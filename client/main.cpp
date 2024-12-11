/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "raylib-cpp.hpp"

int main(void)
{
    Client &client = Client::InitiateInstance("127.0.0.1", 5000);
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    engine.Log(Engine::LogLevel::INFO, "Client started");
    return 0;
}
