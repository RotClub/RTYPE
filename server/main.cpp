/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Engine.hpp"

int main(int argc, char **argv)
{
    try {
        Engine &engine = Engine::StartInstance(Types::VMState::SERVER);
        engine.Log(Engine::LogLevel::INFO, "Server starting...");
        engine.loadLibraries();
        if (engine.LoadLuaFile("index.luau"))
            engine.execute();
        engine.Log(Engine::LogLevel::INFO, "Server started!");
        engine.callHook("RType:InitServer", 0);
    } catch (const std::exception &e) {
        std::cerr << "Could not start server: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
