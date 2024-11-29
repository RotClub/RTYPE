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

    #define MAX_LOGS 50

    #define LUA_PATH "lua/"

class Engine {
    public:
        Engine(Engine &other) = delete;
        void operator=(const Engine &) = delete;
        ~Engine();

        static Engine &GetInstance();

        enum class LogLevel {
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

        std::string GetLuaFileContents(const std::string &filename);
        bool LoadLuaFile(const std::string &filename);
        void execute();

        Node *root;
        std::queue<const log_t> logQueue;

    private:
        Engine();
        static Engine *_instance;

        static std::string _getLogLevelString(LogLevel level);
        lua_State *L;
        std::filesystem::path gamePath;
};

#endif /* !ENGINE_HPP_ */
