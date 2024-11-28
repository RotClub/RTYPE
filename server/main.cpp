/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Engine.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    Engine &engine = Engine::GetInstance();
    engine.Log(Engine::LogLevel::INFO, "Server starting...");
    engine.LoadLuaFile("index.luau");
    engine.execute();
    engine.Log(Engine::LogLevel::INFO, "Server started!");
    return 0;
}
