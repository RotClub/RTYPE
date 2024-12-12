/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ResourceManager
*/

#include "ResourceManager.hpp"

const std::map<std::string, ResourceType> ResourceManager::resourceExtensionsMap = {
    {".png", ResourceType::IMAGE},
    {".jpg", ResourceType::IMAGE},
    {".bmp", ResourceType::IMAGE},
    {".tga", ResourceType::IMAGE},
    {".gif", ResourceType::IMAGE},
    {".qoi", ResourceType::IMAGE},
    {".psd", ResourceType::IMAGE},
    {".dds", ResourceType::IMAGE},
    {".hdr", ResourceType::IMAGE},
    {".ktx", ResourceType::IMAGE},
    {".astc", ResourceType::IMAGE},
    {".pkm", ResourceType::IMAGE},
    {".pvm", ResourceType::IMAGE},
    {".ttf", ResourceType::FONT},
    {".otf", ResourceType::FONT},
    {".wav", ResourceType::SOUND},
    {".ogg", ResourceType::SOUND},
    {".mp3", ResourceType::SOUND},
    {".flac", ResourceType::SOUND},
    {".xm", ResourceType::SOUND},
    {".mod", ResourceType::SOUND},
    {".qoa", ResourceType::SOUND},
};

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

Resource &ResourceManager::loadResource(const std::string &path)
{
    if (_resourceLinks.find(path) != _resourceLinks.end())
        return std::get<1>(_resourceLinks[path]);
    std::string extension = path.substr(path.find_last_of('.'));
    ResourceType type = resourceExtensionsMap.at(extension);
    Resource resource;
    try {
        switch (type) {
            case ResourceType::IMAGE:
                resource = std::make_shared<raylib::Texture>(path);
                break;
            case ResourceType::FONT:
                resource = std::make_shared<raylib::Font>(path);
                break;
            case ResourceType::SOUND:
                resource = std::make_shared<raylib::Sound>(path);
                break;
        }
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to load resource");
    }
    _resourceLinks[path] = std::make_tuple(1, resource, type);
    return std::get<1>(_resourceLinks[path]);
}

void ResourceManager::unloadResource(const std::string &path)
{
    if (_resourceLinks.find(path) == _resourceLinks.end())
        return;
    int &resourceCount = std::get<0>(_resourceLinks[path]);
    resourceCount--;
    if (resourceCount == 0) {
        _resourceLinks.erase(path);
    }
}

Resource& ResourceManager::getResource(const std::string &path)
{
    if (_resourceLinks.find(path) == _resourceLinks.end())
        throw std::runtime_error("Resource not found");
    return std::get<1>(_resourceLinks[path]);
}

raylib::Texture &ResourceManager::getTexture(const std::string &path)
{
    Resource &res = getResource(path);
    if (!std::holds_alternative<std::shared_ptr<raylib::Texture>>(res))
        throw std::runtime_error("Resource is not a texture");
    return *std::get<std::shared_ptr<raylib::Texture>>(res);
}

raylib::Font &ResourceManager::getFont(const std::string &path)
{
    Resource &res = getResource(path);
    if (!std::holds_alternative<std::shared_ptr<raylib::Font>>(res))
        throw std::runtime_error("Resource is not a font");
    return *std::get<std::shared_ptr<raylib::Font>>(res);
}

raylib::Sound &ResourceManager::getSound(const std::string &path)
{
    Resource &res = getResource(path);
    if (!std::holds_alternative<std::shared_ptr<raylib::Sound>>(res))
        throw std::runtime_error("Resource is not a sound");
    return *std::get<std::shared_ptr<raylib::Sound>>(res);
}
