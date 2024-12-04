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
#include "nlohmann/json.hpp"

Engine::Engine(Types::VMState state, const std::string &gamePath)
    : root(nullptr), L(luaL_newstate()), _gamePath("games/" + gamePath), _libPath("libs"), _state(state)
{
    if (!L)
        throw std::runtime_error("Failed to create Lua state");

    std::ifstream manifestFile(_gamePath / "manifest.json");
    nlohmann::json manifestData = nlohmann::json::parse(manifestFile);
    manifestFile.close();

    try {
        _gameInfo = new GameInfo(manifestData);
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to load manifest.json: " + std::string(e.what()));
    }

    luaL_openlibs(L);
    luau_ExposeGameInfoTable(L, _gameInfo);
    luau_ExposeConstants(L, state);
    luau_ExposeFunctions(L);
}

Engine::~Engine()
{
    lua_close(L);
    ClearLogs();
    delete _gameInfo;
}

Engine& Engine::StartInstance(Types::VMState state, const std::string &gamePath)
{
    _instance = new Engine(state, gamePath);
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
    if (level == LogLevel::DEBUG) {
    #ifndef RTYPE_DEBUG
        return;
    #endif
    }
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

void Engine::displayGameInfo()
{
    Log(LogLevel::INFO, "Selected game info:");
    Log(LogLevel::INFO, "Name: " + _gameInfo->getName());
    Log(LogLevel::INFO, "Description: " + _gameInfo->getDescription());
    Log(LogLevel::INFO, "Max players: " + std::to_string(_gameInfo->getMaxPlayers()));
    Log(LogLevel::INFO, "Authors:");
    for (const auto &author : _gameInfo->getAuthors()) {
        Log(LogLevel::INFO, "  - " + author);
    }
    Log(LogLevel::INFO, "Version: " + _gameInfo->getVersion());
}

// WARNING: This function assumes you have already pushed the arguments on the stack
void Engine::callHook(const std::string &eventName, unsigned char numArgs)
{
    lua_getglobal(L, "hook");
    lua_getfield(L, -1, "Call");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushstring(L, eventName.c_str());

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Error calling hook: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

std::string Engine::GetLibraryFileContents(const std::string& filename)
{
    const std::filesystem::path filePath = _libPath / filename;
    if (!std::filesystem::exists(filePath)) {
        Log(LogLevel::ERROR, "File " + filename + " does not exist");
        return "";
    }
    if (filePath.string().find(_libPath.string()) != 0) {
        Log(LogLevel::ERROR, "File " + filename + " is outside the lib path");
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

static void loadLibrary(lua_State *L, const std::string &filePath)
{
    try {
        const std::string &script = Engine::GetInstance().GetLibraryFileContents(filePath);

        size_t bytecodeSize;
        char *bytecode = luau_compile(script.c_str(), script.size(),  nullptr, &bytecodeSize);

        if (!bytecode) {
            throw std::runtime_error("Failed to compile the Lua script.");
        }

        const int loadResult = luau_load(L, filePath.c_str(), bytecode, bytecodeSize, 0);
        free(bytecode);

        if (loadResult != 0) {
            throw std::runtime_error(lua_tostring(L, -1));
        }

        if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
            throw std::runtime_error(lua_tostring(L, -1));
        }

        Engine::GetInstance().Log(Engine::LogLevel::DEBUG, std::format("Successfully loaded file: {}", filePath));
    } catch (const std::exception& e) {
        lua_pushfstring(L, "Exception: %s", e.what());
        lua_error(L);
    }
}

void Engine::loadLibraries() const
{
    loadLibrary(L, "hook.luau");
    loadLibrary(L, "utils.luau");
    loadLibrary(L, "json.luau");
    luaL_sandbox(L);
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
    const std::filesystem::path filePath = _gamePath / LUA_PATH / filename;
    if (!std::filesystem::exists(filePath)) {
        Log(LogLevel::ERROR, "File " + filename + " does not exist");
        return "";
    }
    if (filePath.string().find(_gamePath.string()) != 0) {
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
