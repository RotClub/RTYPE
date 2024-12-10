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

    #include "../submodules/raylib-cpp/include/Texture.hpp"
    #include "../submodules/raylib-cpp/include/Image.hpp"
    #include "../submodules/raylib-cpp/include/Font.hpp"
    #include "../submodules/raylib-cpp/include/Sound.hpp"

    typedef std::variant<raylib::Image, raylib::Texture2D, raylib::Font, raylib::Sound> Resource;

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

            void loadResource(const std::string &name, const std::string &path);
            void unloadResource(const std::string &name);
            Resource &getResource(const std::string &name);
            raylib::Image &getImage(const std::string &name);

        private:
            std::map<std::string, std::tuple<int, Resource, ResourceType>> _resourceLinks;
    };

#endif /* !RESOURCEMANAGER_HPP_ */
