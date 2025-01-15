/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#include "Engine.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "Lua/lua.hpp"

#include "nlohmann/json.hpp"

Engine::Engine(Types::VMState state, const std::string &gamePath) :
    root(new Node("root")), L(luaL_newstate()), _gamePath("games/" + gamePath), _libPath("libs"), _state(state)
{
    if (!L)
        throw std::runtime_error("Failed to create Lua state");

    _timeLast = std::chrono::high_resolution_clock::now();

    std::ifstream manifestFile(_gamePath / "manifest.json");
    nlohmann::json manifestData = nlohmann::json::parse(manifestFile);
    manifestFile.close();

    try {
        _gameInfo = new GameInfo(manifestData);
    }
    catch (const std::exception &e) {
        throw std::runtime_error("Failed to load manifest.json: " + std::string(e.what()));
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    switch (state) {
        case Types::VMState::SERVER:
            sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                "logs/" + _gameInfo->getName() + "/server", 23, 59));
            break;
        case Types::VMState::CLIENT:
            sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                "logs/" + _gameInfo->getName() + "/client", 23, 59));
            break;
    }
    auto engineLogger = std::make_shared<spdlog::logger>("EngineLogger", begin(sinks), end(sinks));
    spdlog::set_default_logger(engineLogger);
    switch (state) {
        case Types::VMState::SERVER:
#ifdef RTYPE_DEBUG
            spdlog::set_pattern("[%Y-%m-%d %T.%e] [SERVER] [%^%-5l%$] <%t> %v");
#else
            spdlog::set_pattern("[%Y-%m-%d %T.%e] [SERVER] [%^%-5l%$] %v");
#endif
            break;
        case Types::VMState::CLIENT:
#ifdef RTYPE_DEBUG
            spdlog::set_pattern("[%Y-%m-%d %T.%e] [CLIENT] [%^%-5l%$] <%t> %v");
#else
            spdlog::set_pattern("[%Y-%m-%d %T.%e] [CLIENT] [%^%-5l%$] %v");
#endif
            break;
    }
    spdlog::flush_every(std::chrono::seconds(5));
#ifdef RTYPE_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
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

Engine &Engine::StartInstance(Types::VMState state, const std::string &gamePath)
{
    _instance = new Engine(state, gamePath);
    if (_instance->L != nullptr)
        luau_ExposeRootNode(_instance->L);
    return *_instance;
}

Engine *Engine::_instance = nullptr;

void Engine::updateNode(Node *root)
{
    root->Update();
    for (auto &child : root->GetChildren()) {
        Engine::updateNode(child);
    }
}

Engine &Engine::GetInstance()
{
    if (_instance == nullptr)
        throw std::runtime_error("Engine instance not started");
    return *_instance;
}

void Engine::addPacketRegistryEntry(const std::string &packetName, const bool reliable)
{
    _packetsRegistry[packetName] = reliable;
    if (_state == Types::VMState::SERVER)
        _newPacketsInRegistry.push(packetName);
}

bool Engine::hasPacketRegistryEntry(const std::string &packetName) const
{
    return _packetsRegistry.contains(packetName);
}

bool Engine::isPacketReliable(const std::string &packetName) const
{
    if (!_packetsRegistry.contains(packetName))
        return false;
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
            lua_pushnumber(L, static_cast<float>(va_arg(args, double)));
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
        spdlog::error("Error calling hook: {}", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void Engine::netCallback(const std::string &packetName, Packet *packet, const std::string &client)
{
    _builders.emplace(packet);
    (void)_builders.top().readString();
    lua_getglobal(L, "net");
    lua_getfield(L, -1, "Call");

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushstring(L, packetName.c_str());
    lua_pushinteger(L, packet->n);
    lua_pushstring(L, client.c_str());

    if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
        spdlog::error("Error calling net callback: {}", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

std::string Engine::GetLibraryFileContents(const std::string &filename)
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
        char *bytecode = luau_compile(script.c_str(), script.size(), nullptr, &bytecodeSize);

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
    }
    catch (const std::exception &e) {
        lua_pushfstring(L, "Exception: %s", e.what());
        lua_error(L);
    }
}

void Engine::loadLibraries()
{
    loadLibrary(L, "hook.luau");
    loadLibrary(L, "utils.luau");
    loadLibrary(L, "json.luau");
    loadLibrary(L, "net.luau");

    /* NET LIBRARY */
    constexpr luaL_Reg netLibrary[] = {{"CreatePacket", luau_NetCreatePacket}, {"Start", luau_NetStart},
                                       {"SendToServer", luau_NetSendToServer}, {"SendToClient", luau_NetSendToClient},
                                       {"Broadcast", luau_NetBroadcast},       {"WriteString", luau_NetWriteString},
                                       {"ReadString", luau_NetReadString},     {"ReadInt", luau_NetReadInt},
                                       {"WriteInt", luau_NetWriteInt},         {"ReadFloat", luau_NetReadFloat},
                                       {"WriteFloat", luau_NetWriteFloat},     {nullptr, nullptr}};
    luau_ExposeFunctionsAsLibrary(L, netLibrary, "net");
    /* NET LIBRARY */
}

int Engine::deltaTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = currentTime - _timeLast;

    _timeLast = currentTime;
    _deltaLast = elapsed.count();
    return elapsed.count();
}

void Engine::lockLuaState() { luaL_sandbox(L); }

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
    char *bytecode = luau_compile(source.c_str(), source.size(), nullptr, &bytecodeSize);
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
    }
    else {
        spdlog::info("Successfully initialized Luau environment");
    }
}
