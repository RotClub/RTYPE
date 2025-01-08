/*
** EPITECH PROJECT, 2025
** Rtype [WSL: Ubuntu]
** File description:
** ArgumentManager
*/

#include "ArgumentManager.hpp"

ArgumentManager::ArgumentManager(int ac, char **av)
{
    _ac = ac;
    _av = av;
}

ArgumentManager::~ArgumentManager()
{
}

bool ArgumentManager::checkArguments()
{
    return false;
}
