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

void ResourceManager::loadResource(const std::string &name, const std::string &path)
{
    if (_resourceLinks.find(name) != _resourceLinks.end()) {
        std::get<0>(_resourceLinks[name])++;
        return;
    }
    std::string extension = path.substr(path.find_last_of('.'));
    ResourceType type = resourceExtensionsMap.at(extension);
    Resource resource;
    try
    {
        switch (type) {
            case ResourceType::IMAGE:
                resource = raylib::Image(path);
                break;
            case ResourceType::FONT:
                resource = raylib::Font(path);
                break;
            case ResourceType::SOUND:
                resource = raylib::Sound(path);
                break;
        }
    }
    catch (const std::exception& e)
    {
        return;
    }
    _resourceLinks[name] = std::make_tuple(1, std::move(resource), type);
}

void ResourceManager::unloadResource(const std::string &name)
{
    if (_resourceLinks.find(name) == _resourceLinks.end())
        return;
    std::get<0>(_resourceLinks[name])--;
    if (std::get<0>(_resourceLinks[name]) == 0)
        _resourceLinks.erase(name);
}

Resource& ResourceManager::getResource(const std::string &name)
{
    if (_resourceLinks.find(name) == _resourceLinks.end())
        throw std::runtime_error("Resource not found");
    return std::get<1>(_resourceLinks[name]);
}

raylib::Image &ResourceManager::getImage(const std::string &name)
{
    if (_resourceLinks.find(name) == _resourceLinks.end())
        throw std::runtime_error("Resource not found");
    if (std::get<2>(_resourceLinks[name]) != ResourceType::IMAGE)
        throw std::runtime_error("Resource is not an image");
    return std::get<raylib::Image>(std::get<1>(_resourceLinks[name]));
}
