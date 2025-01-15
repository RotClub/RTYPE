//
// Created by Théo Bary on 13/01/2025.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <atomic>

class Config
{
    public:
        Config(Config const &) = delete;
        void operator=(Config const &) = delete;

        static Config &GetInstance();

        enum class ColorBlindnessMode
        {
            NONE,
            PROTANOPIA,
            DEUTERANOPIA,
            TRITANOPIA
        };

        void setFpsCounter(bool value) { _fpsCount = value; }
        [[nodiscard]] bool getFpsCounter() const { return _fpsCount; }

        void setColorBlindnessMode(const ColorBlindnessMode mode) { _colorBlindnessMode = mode; }
        [[nodiscard]] ColorBlindnessMode *getColorBlindnessMode() { return &_colorBlindnessMode; }
        void iterateColorBlindnessMode()
        {
            _colorBlindnessMode = static_cast<ColorBlindnessMode>((static_cast<int>(_colorBlindnessMode) + 1) % 4);
        }

    protected:
        Config();

    private:
        bool _fpsCount;
        ColorBlindnessMode _colorBlindnessMode;
};

#endif // CONFIG_HPP
