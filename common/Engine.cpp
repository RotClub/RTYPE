/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#include "Engine.hpp"
#include <iostream>

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

void Engine::Log(const LogLevel level, const std::string &message)
{
    if (logQueue.size() >= MAX_LOGS)
        logQueue.pop();
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::string timestamp = "[" + std::to_string(now->tm_mday) + "-" + std::to_string(now->tm_mon + 1) + "-" + std::to_string(now->tm_year + 1900) + " @ " + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "]";
    std::cout << timestamp << " " << _getLogLevelString(level) << ": " << message << std::endl;
    logQueue.push({
        .level = level,
        .timestamp = timestamp,
        .message = message
    });
}

void Engine::ClearLogs()
{
    while (!logQueue.empty())
        logQueue.pop();
}

std::string Engine::_getLogLevelString(const LogLevel level)
{
    switch (level) {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
    }
    return "UNKNOWN";
}
