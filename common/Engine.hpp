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
    #include <queue>
    #include <string>
    #include <filesystem>
    #include <map>
    #include <GameInfo/GameInfo.hpp>
    #include "Types.hpp"

#define MAX_LOGS 50

    #define LUA_PATH "lua/"

class Engine {
    public:
        Engine(Engine &other) = delete;
        void operator=(const Engine &) = delete;
        ~Engine();

        static Engine &StartInstance(Types::VMState state, const std::string &gamePath);
        static Engine &GetInstance();

        enum class LogLevel {
            DEBUG,
            INFO,
            WARNING,
            ERROR
        };

        typedef struct log_s {
            const LogLevel level;
            const std::string &timestamp;
            const std::string &message;
        } log_t;

        void Log(LogLevel level, const std::string &message);
        void ClearLogs();
        int deltaTime();

        void addPacket(const std::string &packetName, bool reliable);
        [[nodiscard]] bool hasPacket(const std::string &packetName) const;
        [[nodiscard]] bool isPacketReliable(const std::string &packetName) const;

        void displayGameInfo();
        [[nodiscard]] const GameInfo *getGameInfo() const { return _gameInfo; }

        void callHook(const std::string &eventName, ...);

        std::string GetLibraryFileContents(const std::string &filename);
        void loadLibraries() const;

        std::string GetLuaFileContents(const std::string &filename);
        bool LoadLuaFile(const std::string &filename);
        void execute();

        [[nodiscard]] lua_State *getLuaState() const { return L; }

        Node *root;
        std::queue<log_t> logQueue;

    private:
        Engine(Types::VMState state, const std::string &gamePath);
        static Engine *_instance;

        /* NET LIBRARY
         * This value contains all packets registered by the server that need to be sent to the client.
         * The key is the packet name and the value is a boolean that indicates if the packet is reliable or not.
         * true  = reliable   (TCP)
         * false = unreliable (UDP)
         */
        std::map<std::string, bool> _packetsRegistry;

        static std::string _getLogLevelString(LogLevel level);
        lua_State *L;
        std::filesystem::path _gamePath;
        std::filesystem::path _libPath;
        Types::VMState _state;
        std::chrono::time_point<std::chrono::high_resolution_clock> _deltaLast;
        const GameInfo *_gameInfo;
};

#endif /* !ENGINE_HPP_ */
