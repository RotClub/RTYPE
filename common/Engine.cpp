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
#include <unistd.h>
#include <chrono>

#include "nlohmann/json.hpp"

Engine::Engine(Types::VMState state, const std::string &gamePath)
    : root(new Node("root")), L(luaL_newstate()), _gamePath("games/" + gamePath), _libPath("libs"), _state(state)
{
    if (!L)
        throw std::runtime_error("Failed to create Lua state");

    _deltaLast = std::chrono::high_resolution_clock::now();

    std::ifstream manifestFile(_gamePath / "manifest.json");
    nlohmann::json manifestData = nlohmann::json::parse(manifestFile);
    manifestFile.close();

    try {
        _gameInfo = new GameInfo(manifestData);
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to load manifest.json: " + std::string(e.what()));
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/" + _gameInfo->getName(), 23, 59));
    auto engineLogger = std::make_shared<spdlog::logger>("EngineLogger", begin(sinks), end(sinks));
    spdlog::set_default_logger(engineLogger);
    spdlog::flush_on(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(5));
    #ifdef RTYPE_DEBUG
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%-5l%$] <%t> %v");
    #else
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%-5l%$] %v");
    #endif

    luaL_openlibs(L);
    luau_ExposeGameInfoTable(L, _gameInfo);
    luau_ExposeConstants(L, state);
    luau_ExposeFunctions(L);
}

Engine::~Engine()
{
    lua_close(L);
    delete _gameInfo;
}

Engine& Engine::StartInstance(Types::VMState state, const std::string &gamePath)
{
    _instance = new Engine(state, gamePath);
    if (_instance->L != nullptr)
        luau_ExposeRootNode(_instance->L);
    return *_instance;
}

Engine *Engine::_instance = nullptr;

Engine &Engine::GetInstance()
{
    if (_instance == nullptr)
        throw std::runtime_error("Engine instance not started");
    return *_instance;
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
    spdlog::info("Selected game info:");
    spdlog::info("Name: " + _gameInfo->getName());
    spdlog::info("Description: " + _gameInfo->getDescription());
    spdlog::info("Max players: " + std::to_string(_gameInfo->getMaxPlayers()));
    spdlog::info("Authors:");
    for (const auto &author : _gameInfo->getAuthors()) {
        spdlog::info("  - " + author);
    }
    spdlog::info("Version: " + _gameInfo->getVersion());
}

// WARNING: This function requires the last argument to be nullptr
// The arguments must be in pairs of type and value
// Accepted types are: "int", "float", "double", "string", "boolean"
// Example: callHook("RType:InitServer", "int", 42, nullptr);
void Engine::callHook(const std::string &eventName, ...)
{
    lua_getglobal(L, "hook");
    lua_getfield(L, -1, "Call");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushstring(L, eventName.c_str());

    va_list args;
    va_start(args, eventName);
    int argCount = 0;
    while (true) {
        const char *type = va_arg(args, const char *);
        if (type == nullptr)
            break;
        if (strcmp(type, "int") == 0)
            lua_pushinteger(L, va_arg(args, int));
        else if (strcmp(type, "float") == 0)
            lua_pushnumber(L, va_arg(args, float));
        else if (strcmp(type, "double") == 0)
            lua_pushnumber(L, va_arg(args, double));
        else if (strcmp(type, "string") == 0)
            lua_pushstring(L, va_arg(args, const char *));
        else if (strcmp(type, "boolean") == 0)
            lua_pushboolean(L, va_arg(args, int));
        else
            throw std::runtime_error("Invalid LuaType");
        argCount++;
    }
    va_end(args);

    if (lua_pcall(L, 1 + argCount, 0, 0) != LUA_OK) {
        std::cerr << "Error calling hook: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

std::string Engine::GetLibraryFileContents(const std::string& filename)
{
    const std::filesystem::path filePath = _libPath / filename;
    if (!std::filesystem::exists(filePath)) {
        spdlog::error("File " + filename + " does not exist");
        return "";
    }
    if (filePath.string().find(_libPath.string()) != 0) {
        spdlog::error("File " + filename + " is outside the lib path");
        return "";
    }
    std::ifstream stream(filePath.c_str(), std::ios::binary);
    if (!stream.is_open()) {
        spdlog::error("Failed to open file " + filename);
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

        spdlog::debug("Successfully loaded file: {}", filePath);
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

int Engine::deltaTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = currentTime - _deltaLast;

    _deltaLast = currentTime;
    return elapsed.count();
}

std::string Engine::GetLuaFileContents(const std::string &filename)
{
    const std::filesystem::path filePath = _gamePath / LUA_PATH / filename;
    if (!std::filesystem::exists(filePath)) {
        spdlog::error("File " + filename + " does not exist");
        return "";
    }
    if (filePath.string().find(_gamePath.string()) != 0) {
        spdlog::error("File " + filename + " is outside the game path");
        return "";
    }
    std::ifstream stream(filePath.c_str(), std::ios::binary);
    if (!stream.is_open()) {
        spdlog::error("Failed to open file " + filename);
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
        spdlog::error("Failed to load Lua file " + filename);
        return false;
    }
    return true;
}

void Engine::execute()
{
    luaL_sandboxthread(L);
    if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
        spdlog::error(lua_tostring(L, -1));
    } else {
        spdlog::info("Successfully initialized Luau environment");
    }
}
