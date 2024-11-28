/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Engine
*/

#ifndef ENGINE_HPP_
    #define ENGINE_HPP_

    #include "Nodes/Node.hpp"
    #include <queue>
    #include <utility>
    #include <chrono>
    #include <ctime>

    #define MAX_LOGS 50

class Engine {
    public:
        Engine();
        ~Engine();

        static const Engine &GetInstance();

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

        void Log(const LogLevel level, const std::string &message);
        void ClearLogs();

        Node *root;
        std::queue<const log_t> logQueue;

    private:
        std::string _getLogLevelString(const LogLevel level);
};

#endif /* !ENGINE_HPP_ */
