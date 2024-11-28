/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Engine.hpp"

int main(int argc, char **argv)
{
    Engine engine = Engine::GetInstance();
    engine.Log(Engine::LogLevel::INFO, "Hello, World!");
    return 0;
}
