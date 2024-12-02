/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Engine.hpp"

int main(int argc, char **argv)
{
    Engine &engine = Engine::StartInstance(Types::VMState::SERVER);
    engine.Log(Engine::LogLevel::INFO, "Server starting...");
    if (engine.LoadLuaFile("index.luau"))
        engine.execute();
    engine.Log(Engine::LogLevel::INFO, "Server started!");
    return 0;
}
