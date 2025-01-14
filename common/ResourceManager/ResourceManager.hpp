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

    #include "../submodules/raylib-cpp/include/raylib-cpp.hpp"

    using Resource = std::variant<
        std::shared_ptr<raylib::Texture>,
        std::shared_ptr<raylib::Font>,
        std::shared_ptr<raylib::Sound>,
        std::shared_ptr<raylib::Shader>,
        std::shared_ptr<std::string>
    >;

    enum class ResourceType {
        IMAGE,
        FONT,
        SOUND,
        TEXT,
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    class ResourceManager {
        public:
            ResourceManager();
            ~ResourceManager();

            static const std::map<std::string, ResourceType> resourceExtensionsMap;

            Resource &loadResource(const std::string &path);
            void unloadResource(const std::string &path);
            void addPendingResource(const std::string &path);
            void loadAllPendingResources();
            Resource &getResource(const std::string &path);
            raylib::Texture &getTexture(const std::string &path);
            raylib::Font &getFont(const std::string &path);
            raylib::Sound &getSound(const std::string &path);
            raylib::Shader &getVertexShader(const std::string &path);
            raylib::Shader &getFragmentShader(const std::string &path);

            static const Resource &nullResourceRef;

        private:
            std::vector<std::string> _resourceToLoad;
            std::map<std::string, std::tuple<int, Resource, ResourceType>> _resourceLinks;

    };

#endif /* !RESOURCEMANAGER_HPP_ */
