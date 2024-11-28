/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"

int main(void) {
    Client *client = Client::InitiateInstance("127.0.0.1", 5000);
    return 0;
}
