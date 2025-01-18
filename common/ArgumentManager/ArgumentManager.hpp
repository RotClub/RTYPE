/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu-20.04]
** File description:
** ArgumentManager
*/

#ifndef ARGUMENTMANAGER_HPP_
    #define ARGUMENTMANAGER_HPP_

    #include <string>
    #include <regex>
    #include <spdlog/spdlog.h>

    class ArgumentManager {
        public:
            ArgumentManager(int argc, char **argv);
            ~ArgumentManager();
            bool checkServerArguments();
            bool checkClientArguments();

        private:
            int _argc;
            char **_argv;
            bool validateIp(const std::string &ip);
            bool validatePort(const std::string &portStr, unsigned long min, unsigned long max);
    };

#endif /* !ARGUMENTMANAGER_HPP_ */
