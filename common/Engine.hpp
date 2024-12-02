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
    #include <filesystem>
    #include <map>

    #define MAX_LOGS 50

    #define LUA_PATH "lua/"

class Engine {
    public:
        Engine(Engine &other) = delete;
        void operator=(const Engine &) = delete;
        ~Engine();

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

        void addPacket(const std::string &packetName, bool reliable);
        bool hasPacket(const std::string &packetName) const;
        bool isPacketReliable(const std::string &packetName) const;

        std::string GetLuaFileContents(const std::string &filename);
        bool LoadLuaFile(const std::string &filename);
        void execute();

        Node *root;
        std::queue<const log_t> logQueue;

    private:
        Engine();
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
        std::filesystem::path gamePath;
};

#endif /* !ENGINE_HPP_ */
