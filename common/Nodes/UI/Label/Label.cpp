//
// Created by Yanis Moyon on 14/01/2025.
//

#include "Label.hpp"
#include "Engine.hpp"

Label::Label(const std::string &name, const std::string &text, const std::string &font, int fontSize) : UI(name, {0, 0}), _text(text), _fontSize(fontSize)
{
    if (!font.empty())
        _font = Engine::GetInstance().getGamePath() / font;
        Engine::GetInstance().getResourceManager().loadResource(_font);
}

Label::~Label()
{
}

void Label::Draw()
{
    raylib::Font &font = Engine::GetInstance().getResourceManager().getFont(_font);
    raylib::Color color = raylib::Color(red, green, blue, alpha);

    raylib::DrawTextEx(font, _text.c_str(), {getGlobalPosition().x, getGlobalPosition().y}, _fontSize, 0, color);
}
