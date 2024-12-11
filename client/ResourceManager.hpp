/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ResourceManager
*/

#ifndef RESOURCEMANAGER_HPP_
    #define RESOURCEMANAGER_HPP_

    #include <map>
    #include <tuple>
    #include <string>
    #include <variant>
    #include <memory>

    #include "../submodules/raylib-cpp/include/Texture.hpp"
    #include "../submodules/raylib-cpp/include/Image.hpp"
    #include "../submodules/raylib-cpp/include/Font.hpp"
    #include "../submodules/raylib-cpp/include/Sound.hpp"

    typedef std::variant<std::shared_ptr<raylib::Texture>, std::shared_ptr<raylib::Font>, std::shared_ptr<raylib::Sound>> Resource;

    enum class ResourceType {
        IMAGE,
        FONT,
        SOUND
    };

    class ResourceManager {
        public:
            ResourceManager();
            ~ResourceManager();

            static const std::map<std::string, ResourceType> resourceExtensionsMap;

            Resource &loadResource(const std::string &path);
            void unloadResource(const std::string &path);
            Resource &getResource(const std::string &path);
            raylib::Texture &getTexture(const std::string &path);
            raylib::Font &getFont(const std::string &path);
            raylib::Sound &getSound(const std::string &path);

        private:
            std::map<std::string, std::tuple<int, Resource, ResourceType>> _resourceLinks;
    };

#endif /* !RESOURCEMANAGER_HPP_ */
