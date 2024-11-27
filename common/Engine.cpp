/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#include "Engine.hpp"

Engine::Engine()
    : root(nullptr)
{
}

Engine::~Engine()
{
}

const Engine &Engine::GetInstance()
{
    static Engine instance;
    return instance;
}

void Engine::Log(const std::string &message)
{
    if (logQueue.size() >= MAX_LOGS)
        logQueue.pop();
    const time_t now = std::mktime(nullptr);
    logQueue.push(std::make_pair(std::localtime(&now), message));
}

void Engine::ClearLogs()
{
    while (!logQueue.empty())
        logQueue.pop();
}
