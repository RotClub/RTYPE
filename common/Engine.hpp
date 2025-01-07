/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#ifndef ENGINE_HPP_
    #define ENGINE_HPP_

    #include "Nodes/Node.hpp"
    #include "lua.h"
    #include "luaconf.h"
    #include "lualib.h"
    #include "luacode.h"
    #include "Lua/lua.hpp"
    #include "ResourceManager/ResourceManager.hpp"
    #include <queue>
    #include <string>
    #include <filesystem>
    #include <map>
    #include "GameInfo/GameInfo.hpp"
    #include "Networking/PacketBuilder.hpp"
    #include "spdlog/spdlog.h"
    #include "spdlog/sinks/stdout_color_sinks.h"
    #include "spdlog/sinks/daily_file_sink.h"
    #include "Types.hpp"

    #define LUA_PATH "lua/"

    #define SERVER_CHALLENGE "SERVER-YNwUJMvHMf09J0R1"
    #define CLIENT_CHALLENGE "CLIENT-qtZzAo4HL71h0iMt"

class Engine {
    public:
        Engine(Engine &other) = delete;
        void operator=(const Engine &) = delete;
        ~Engine();

        static Engine &StartInstance(Types::VMState state, const std::string &gamePath);
        static Engine &GetInstance();

        int deltaTime();

        void addPacketRegistryEntry(const std::string &packetName, bool reliable);
        [[nodiscard]] bool hasPacketRegistryEntry(const std::string &packetName) const;
        [[nodiscard]] bool isPacketReliable(const std::string &packetName) const;

        [[nodiscard]] const std::filesystem::path &getGamePath() const { return _gamePath; }

        [[nodiscard]] Types::VMState getState() const { return _state; }

        void displayGameInfo();
        [[nodiscard]] const GameInfo *getGameInfo() const { return _gameInfo; }

        [[nodiscard]] ResourceManager &getResourceManager() { return _resourceManager; }

        void callHook(const std::string &eventName, ...);

        std::string GetLibraryFileContents(const std::string &filename);
        void loadLibraries() const;

        std::string GetLuaFileContents(const std::string &filename);
        bool LoadLuaFile(const std::string &filename);
        void execute();

        [[nodiscard]] lua_State *getLuaState() const { return L; }

        [[nodiscard]] const std::unordered_map<std::string, bool> &getPacketsRegistry() const { return _packetsRegistry; }
        bool hasNewPacketToBroadcast() const { return !_newPacketsInRegistry.empty(); }
        std::queue<std::string> &getNewPacketsInRegistry() { return _newPacketsInRegistry; }
        std::queue<std::pair<std::string, Packet *>> &getBroadcastQueue() { return _broadcastQueue; }
        std::unordered_map<std::string, std::queue<std::pair<std::string, Packet *>>> &getSendToClientMap() { return _sendToClientQueue; }

        PacketBuilder &getPacketBuilder() { return _builder; }
        std::string &getLastStartedPacket() { return _lastStartedPacket; }

        void netCallback(const std::string &packetName, Packet *packet, const std::string &client);

        Node *root;
        bool clientStarted = false;

    private:
        Engine(Types::VMState state, const std::string &gamePath);
        static Engine *_instance;

        /* NET LIBRARY
         * This value contains all packets registered by the server that need to be sent to the client.
         * The key is the packet name and the value is a boolean that indicates if the packet is reliable or not.
         * true  = reliable   (TCP)
         * false = unreliable (UDP)
         */
        std::unordered_map<std::string, bool> _packetsRegistry;
        std::queue<std::string> _newPacketsInRegistry;

        std::queue<std::pair<std::string, Packet *>> _broadcastQueue;
        std::unordered_map<std::string, std::queue<std::pair<std::string, Packet *>>> _sendToClientQueue;

        lua_State *L;
        std::filesystem::path _gamePath;
        std::filesystem::path _libPath;
        Types::VMState _state;
        std::chrono::high_resolution_clock::time_point _deltaLast;
        const GameInfo *_gameInfo;
        ResourceManager _resourceManager;
        std::string _lastStartedPacket;
        PacketBuilder _builder;
};

#endif /* !ENGINE_HPP_ */
