/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#include "Engine.hpp"
#include <iostream>
#include <ctime>
#include <fstream>

Engine::Engine(Types::VMState state)
    : root(nullptr), L(luaL_newstate()), gamePath("games/rtype"), _state(state)
{
    luaL_openlibs(L);
    luau_ExposeConstants(L, state);
    luau_ExposeFunctions(L);
    luaL_sandbox(L);
}

Engine::~Engine()
{
    lua_close(L);
    ClearLogs();
}

Engine& Engine::StartInstance(Types::VMState state)
{
    _instance = new Engine(state);
    return *_instance;
}

Engine *Engine::_instance = nullptr;

Engine &Engine::GetInstance()
{
    if (_instance == nullptr)
        throw std::runtime_error("Engine instance not started");
    return *_instance;
}

void Engine::Log(const LogLevel level, const std::string &message)
{
    if (logQueue.size() >= MAX_LOGS)
        logQueue.pop();
    const std::time_t t = std::time(nullptr);
    const std::tm* now = std::localtime(&t);
    const std::string timestamp = "[" + std::to_string(now->tm_mday) + "-" + std::to_string(now->tm_mon + 1) + "-" + std::to_string(now->tm_year + 1900) + " @ " + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "]";
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

void Engine::addPacket(const std::string &packetName, const bool reliable)
{
    _packetsRegistry[packetName] = reliable;
    // TODO: Network packet creation to all clients if SERVER. if CLIENT, just create the packet
}

bool Engine::hasPacket(const std::string &packetName) const
{
    return _packetsRegistry.contains(packetName);
}

bool Engine::isPacketReliable(const std::string &packetName) const
{
    return _packetsRegistry.at(packetName);
}

std::string Engine::_getLogLevelString(const LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
    }
    return "UNKNOWN";
}

std::string Engine::GetLuaFileContents(const std::string &filename)
{
    const std::filesystem::path filePath = gamePath / LUA_PATH / filename;
    if (!std::filesystem::exists(filePath)) {
        Log(LogLevel::ERROR, "File " + filename + " does not exist");
        return "";
    }
    if (filePath.string().find(gamePath.string()) != 0) {
        Log(LogLevel::ERROR, "File " + filename + " is outside the game path");
        return "";
    }
    std::ifstream stream(filePath.c_str(), std::ios::binary);
    if (!stream.is_open()) {
        Log(LogLevel::ERROR, "Failed to open file " + filename);
        return "";
    }
    std::string contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    stream.close();
    return contents;
}

bool Engine::LoadLuaFile(const std::string &filename)
{
    std::string source = GetLuaFileContents(filename);
    if (source.empty())
        return false;
    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(source.c_str(), source.size(), nullptr, &bytecodeSize);
    const int result = luau_load(L, filename.c_str(), bytecode, bytecodeSize, 0);
    free(bytecode);
    if (result != 0) {
        Log(LogLevel::ERROR, "Failed to load Lua file " + filename);
        return false;
    }
    return true;
}

void Engine::execute()
{
    luaL_sandboxthread(L);
    if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
        Log(LogLevel::ERROR, lua_tostring(L, -1));
    }
}
