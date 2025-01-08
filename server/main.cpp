/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"
#include "argumentManager/ArgumentManager.hpp"

int main(int argc, char **argv)
{
    try {
        ArgumentManager argManager(argc, argv);
        if (!argManager.checkArguments())
            return 84;
        Server srv(argc == 1 ? 25777 : std::stoi(argv[2]));

        srv.start();
    } catch (const std::exception &e) {
        std::cerr << "Could not start server: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
