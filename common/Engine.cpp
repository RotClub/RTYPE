/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#include "Engine.hpp"
#include <iostream>
#include <ctime>
#include <iostream>
#include <fstream>

#include "../submodules/luau/VM/src/lvm.h"

Engine::Engine()
    : root(nullptr), L(luaL_newstate()), gamePath("games/rtype")
{
    luaL_openlibs(L);
}

Engine::~Engine()
{
    // lua_close(L);
    ClearLogs();
}

Engine *Engine::_instance = nullptr;

Engine &Engine::GetInstance()
{
    if (_instance == nullptr)
        _instance = new Engine();
    return *_instance;
}

void Engine::Log(const LogLevel level, const std::string &message)
{
    if (logQueue.size() >= MAX_LOGS)
        logQueue.pop();
    std::time_t t = std::time(nullptr);
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

std::string Engine::_getFileContents(const std::string &filename)
{
    const std::filesystem::path filePath = gamePath / LUA_PATH / filename;
    std::cout << filePath << std::endl;
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
    std::string source = _getFileContents(filename);
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

void Engine::execute() const
{
    lua_pcall(L, 0, 0, 0);
}
