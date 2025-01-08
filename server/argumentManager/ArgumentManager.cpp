/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu]
** File description:
** ArgumentManager
*/

#include "ArgumentManager.hpp"

ArgumentManager::ArgumentManager(int ac, char **av)
{
    _ac = ac;
    _av = av;
}

ArgumentManager::~ArgumentManager()
{
}

bool ArgumentManager::checkArguments()
{
    if (_ac == 1)
        return true;
    if (_ac != 3) {
        spdlog::error("Usage: ./rtype_server --port [port]");
        return false;
    }
    if (std::string(_av[1]) != "--port") {
        spdlog::error("Usage: ./rtype_server --port [port]");
        return false;
    }
    for (int i = 0; _av[2][i]; i++) {
        if (!std::isdigit(_av[2][i])) {
            spdlog::error("Port must be a valid number");
            return false;
        }
    }
    try {
        unsigned long port = std::stoul(_av[2]);
        if (port > 65535) {
            spdlog::error("Port must be between 0 and 65535");
            return false;
        }
    } catch (const std::invalid_argument&) {
        spdlog::error("Invalid port: not a number");
        return false;
    } catch (const std::out_of_range&) {
        spdlog::error("Invalid port: out of range");
        return false;
    }
    return true;
}
