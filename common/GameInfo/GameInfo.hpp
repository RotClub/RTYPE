//
// Created by Théo Bary on 04/12/2024.
//

#ifndef GAMEINFO_HPP
#define GAMEINFO_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class GameInfo
{
    public:
        GameInfo(const std::string &name, const std::string &description, unsigned int max_players,
                 const std::vector<std::string> &authors, const std::string &version);
        explicit GameInfo(nlohmann::json &gameData);
        ~GameInfo() = default;

        [[nodiscard]] const std::string &getName() const { return _name; }
        [[nodiscard]] const std::string &getDescription() const { return _description; }
        [[nodiscard]] unsigned int getMaxPlayers() const { return _max_players; }
        [[nodiscard]] const std::vector<std::string> &getAuthors() const { return _authors; }
        [[nodiscard]] const std::string &getVersion() const { return _version; }

    private:
        const std::string _name;
        const std::string _description;
        const unsigned int _max_players;
        std::vector<std::string> _authors;
        const std::string _version;
};

#endif // GAMEINFO_HPP
