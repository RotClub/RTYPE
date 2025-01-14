/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** ClientArgumentManager
*/

#include "ClientArgumentManager.hpp"

ClientArgumentManager::ClientArgumentManager(int argc, char **argv) : _argc(argc), _argv(argv) {}

ClientArgumentManager::~ClientArgumentManager() {}

bool ClientArgumentManager::checkClientArguments()
{
    if (_argc != 5) {
        spdlog::error("Usage: ./rtype_client --ip [ip] --port [port]");
        return false;
    }
    if (std::string(_argv[1]) != "--ip" || std::string(_argv[3]) != "--port") {
        spdlog::error("Usage: ./rtype_client --ip [ip] --port [port]");
        return false;
    }
    std::string ip = _argv[2];
    // regex pattern for IPv4 address = (0-255).(0-255).(0-255).(0-255)
    std::regex ip_pattern(R"(^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$)");

    if (!std::regex_match(ip, ip_pattern)) {
        spdlog::error("Invalid ip: IP must be a valid IPv4 address");
        return false;
    }
    try {
        unsigned long port = std::stoul(_argv[4]);
        if (port < 1024 || port > 65535) {
            spdlog::error("Invalid port: must be between 1024 and 65535");
            return false;
        }
    }
    catch (std::exception &e) {
        spdlog::error("Invalid port: must be a valid number between 1024 and 65535");
        return false;
    }
    return true;
}
