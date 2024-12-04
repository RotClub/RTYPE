//
// Created by Théo Bary on 04/12/2024.
//

#ifndef GAMEINFO_HPP
    #define GAMEINFO_HPP

    #include <string>
    #include <vector>
    #include <nlohmann/json.hpp>

class GameInfo {
    public:
        GameInfo(
            const std::string &name,
            const std::string &description,
            unsigned int max_players,
            const std::vector<const std::string>& authors,
            float version
        );
        explicit GameInfo(nlohmann::json &gameData);
        ~GameInfo() = default;

        [[nodiscard]] const std::string &getName() const { return _name; }
        [[nodiscard]] const std::string &getDescription() const { return _description; }
        [[nodiscard]] unsigned int getMaxPlayers() const { return _max_players; }
        [[nodiscard]] const std::vector<const std::string> &getAuthors() const { return _authors; }
        [[nodiscard]] float getVersion() const { return _version; }

    private:
        const std::string _name;
        const std::string _description;
        const unsigned int _max_players;
        std::vector<const std::string> _authors;
        const float _version;
};

#endif //GAMEINFO_HPP
