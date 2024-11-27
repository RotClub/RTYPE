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
    #include <ctime>

    #define MAX_LOGS 50

class Engine {
    public:
        Engine();
        ~Engine();

        const Engine &GetInstance();

        void Log(const std::string &message);
        void ClearLogs();

        Node *root;
        std::queue< std::pair<std::tm *, std::string> > logQueue;
};

#endif /* !ENGINE_HPP_ */
