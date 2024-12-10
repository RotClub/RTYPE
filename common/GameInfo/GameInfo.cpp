//
// Created by Théo Bary on 04/12/2024.
//

#include "GameInfo.hpp"

GameInfo::GameInfo(
    const std::string &name,
    const std::string &description,
    const unsigned int max_players,
    const std::vector<std::string> &authors,
    const std::string &version
) : _name(name), _description(description), _max_players(max_players), _authors(authors), _version(version)
{
}

GameInfo::GameInfo(nlohmann::json& gameData)
    :   _name(gameData["name"]),
        _description(gameData["description"]),
        _max_players(gameData["max_players"]),
        _authors({}),
        _version(gameData["version"])
{
    for (const auto &author : gameData["authors"]) {
        _authors.push_back(author);
    }
}
