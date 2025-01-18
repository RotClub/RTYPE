/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu-20.04]
** File description:
** ArgumentManager
*/

#include "ArgumentManager.hpp"

ArgumentManager::ArgumentManager(int argc, char **argv) : _argc(argc), _argv(argv) {}

ArgumentManager::~ArgumentManager() {}

bool ArgumentManager::checkServerArguments()
{
    if (_argc == 1)
        return true;
    if (_argc != 5) {
        spdlog::error("Usage: ./rtype_server --port [port] --game [game name]");
        return false;
    }
    if (std::string(_argv[1]) != "--port" || std::string(_argv[3]) != "--game") {
        spdlog::error("Usage: ./rtype_server --port [port] --game [game name]");
        return false;
    }
    if (!validatePort(_argv[2], 0, 65535)) {
        return false;
    }
    if (!validateGameName(_argv[4])) {
        return false;
    }
    return true;
}

bool ArgumentManager::checkClientArguments()
{
    if (_argc != 5) {
        spdlog::error("Usage: ./rtype_client --ip [ip] --port [port]");
        return false;
    }
    if (std::string(_argv[1]) != "--ip" || std::string(_argv[3]) != "--port") {
        spdlog::error("Usage: ./rtype_client --ip [ip] --port [port]");
        return false;
    }
    if (!validateIp(_argv[2])) {
        spdlog::error("Invalid IP: must be a valid IPv4 address");
        return false;
    }
    return validatePort(_argv[4], 1024, 65535);
}

bool ArgumentManager::validateIp(const std::string &ip)
{
    std::regex ip_pattern(R"(^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$)");
    return std::regex_match(ip, ip_pattern);
}

bool ArgumentManager::validatePort(const std::string &portStr, unsigned long min, unsigned long max)
{
    for (int i = 0; portStr[i]; i++) {
        if (!std::isdigit(portStr[i])) {
            spdlog::error("Port must be a valid number");
            return false;
        }
    }
    try {
        unsigned long port = std::stoul(portStr);
        if (port < min || port > max) {
            spdlog::error("Invalid port: must be between {} and {}", min, max);
            return false;
        }
    }
    catch (const std::invalid_argument &) {
        spdlog::error("Invalid port: not a number");
        return false;
    }
    catch (const std::out_of_range &) {
        spdlog::error("Invalid port: out of range");
        return false;
    }
    return true;
}

bool ArgumentManager::validateGameName(const std::string &gameName)
{
    if (gameName.empty()) {
        return false;
    }
    std::filesystem::path gamePath = std::filesystem::path("games") / gameName;
    if (!std::filesystem::exists(gamePath) || !std::filesystem::is_directory(gamePath)) {
        spdlog::error("Game directory '{}' does not exist in 'games/'", gamePath.string());
        return false;
    }
    return true;
}