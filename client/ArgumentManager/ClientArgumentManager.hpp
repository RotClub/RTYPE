/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** ClientArgumentManager
*/

#ifndef CLIENTARGUMENTMANAGER_HPP_
#define CLIENTARGUMENTMANAGER_HPP_

class ClientArgumentManager {
    public:
        ClientArgumentManager(int argc, char **argv);
        ~ClientArgumentManager();

        bool checkClientArguments();

    private:
        int _argc;
        char **_argv;
};

#endif /* !CLIENTARGUMENTMANAGER_HPP_ */
