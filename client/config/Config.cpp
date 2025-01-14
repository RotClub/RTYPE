//
// Created by Théo Bary on 13/01/2025.
//

#include "Config.hpp"

Config::Config() : _fpsCount(false), _colorBlindnessMode(ColorBlindnessMode::NONE) {}

Config& Config::GetInstance()
{
    static Config instance;
    return instance;
}
