/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu-20.04]
** File description:
** ArgumentManager
*/

#include "ArgumentManager.hpp"

ArgumentManager::ArgumentManager(int argc, char **argv) : _argc(argc), _argv(argv) {}

void ArgumentManager::addRequiredArgument(const std::string &argumentName)
{
    _requiredArguments.push_back(argumentName);
}

void ArgumentManager::addDefaultArgument(const std::string &argumentName, const std::string &defaultValue)
{
    _defaultArguments.emplace(argumentName, defaultValue);
}

void ArgumentManager::parseArguments()
{
    for (int i = 1; i < _argc; i++) {
        if (std::string(_argv[i]).substr(0, 2) == "--") {
            std::string argumentName = std::string(_argv[i]).substr(2);
            if (i + 1 < _argc && std::string(_argv[i + 1]).substr(0, 2) != "--") {
                _arguments[argumentName] = _argv[i + 1];
            }
        }
    }
    for (const auto &requiredArgument : _requiredArguments) {
        if (!_arguments.contains(requiredArgument)) {
            throw ArgumentManagerException("Missing required argument: --" + requiredArgument);
        }
    }
    for (const auto &defaultArgument : _defaultArguments) {
        if (!_arguments.contains(defaultArgument.first)) {
            _arguments[defaultArgument.first] = defaultArgument.second;
        }
    }
}

void ArgumentManager::DisplayServerUsage()
{
    spdlog::info("Usage of ./rtype_server:");
    spdlog::info("  --port [port] : port to listen on (default: 25777)");
    spdlog::info("  --game [game name] : game to load from 'games/' directory");
}

void ArgumentManager::DisplayClientUsage()
{
    spdlog::info("Usage of ./rtype_client:");
    spdlog::info("  --ip [ip] : server IP address");
    spdlog::info("  --port [port] : server port");
    spdlog::info("  --colorblindness [none|protanopia|deuteranopia|tritanopia] : colorblindness mode (default: none)");
}

const std::string &ArgumentManager::getArgument(const std::string &argumentName) const
{
    if (!_arguments.contains(argumentName))
        throw ArgumentManagerException("Argument not found: " + argumentName);
    return _arguments.at(argumentName);
}

bool ArgumentManager::checkServerArguments() const
{
    if (!validatePort()) {
        return false;
    }
    if (!validateGameName()) {
        return false;
    }
    return true;
}

bool ArgumentManager::checkClientArguments() const
{
    if (!validateIp()) {
        spdlog::error("Invalid IP: must be a valid IPv4 address");
        return false;
    }
    return validatePort();
}

bool ArgumentManager::validateIp() const
{
    std::regex ip_pattern(R"(^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$)");
    return std::regex_match(getArgument("ip"), ip_pattern);
}

bool ArgumentManager::validatePort() const
{
    const std::string &portStr = getArgument("port");
    const unsigned int min = 1024;
    const unsigned int max = 65535;
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

bool ArgumentManager::validateGameName() const
{
    if (getArgument("game").empty()) {
        return false;
    }
    std::filesystem::path gamePath = std::filesystem::path("games") / getArgument("game");
    if (!std::filesystem::exists(gamePath) || !std::filesystem::is_directory(gamePath)) {
        spdlog::error("Game directory '{}' does not exist in 'games/'", gamePath.string());
        return false;
    }
    return true;
}