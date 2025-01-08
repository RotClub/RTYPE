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
        if (_av[2][i] < '0' || _av[2][i] > '9') {
            spdlog::error("Usage: ./rtype_server --port [port]");
            return false;
        }
    }
    if (std::stoi(_av[2]) < 0 || std::stoi(_av[2]) > 65535) {
        spdlog::error("Port must be between 0 and 65535");
        return false;
    }
    return true;
}
