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
    #include <filesystem>
    #include <map>
    #include <exception>
#include <utility>

class ArgumentManager {
    public:
        ArgumentManager(int argc, char **argv);
        ~ArgumentManager() = default;
        void addRequiredArgument(const std::string &argumentName);
        void addDefaultArgument(const std::string &argumentName, const std::string &defaultValue);
        void parseArguments();
        static void DisplayServerUsage();
        bool checkServerArguments() const;
        static void DisplayClientUsage();
        bool checkClientArguments() const;

        [[nodiscard]] const std::string &getArgument(const std::string &argumentName) const;

        class ArgumentManagerException final : public std::exception {
            public:
                explicit ArgumentManagerException(std::string message) : _message(std::move(message)) {}
                [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); }
            private:
                std::string _message;
        };

    private:
        int _argc;
        char **_argv;
        bool validateIp() const;
        bool validatePort() const;
        bool validateGameName() const;
        std::vector<std::string> _requiredArguments;
        std::map<std::string, std::string> _defaultArguments;
        std::map<std::string, std::string> _arguments;
};
#endif /* !ARGUMENTMANAGER_HPP_ */
