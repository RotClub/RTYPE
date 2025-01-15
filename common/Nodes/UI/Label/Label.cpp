//
// Created by Yanis Moyon on 14/01/2025.
//

#include "Label.hpp"
#include "Engine.hpp"

Label::Label(const std::string &name, const Types::Vector2 &position, const std::string &text, const std::string &font, int fontSize) : UI(name, position), _text(text), _fontSize(fontSize)
{
    if (!font.empty()) {
        _font = Engine::GetInstance().getGamePath() / font;
        Engine::GetInstance().getResourceManager().loadResource(_font);
    }
}

Label::~Label()
{
}

void Label::Draw()
{
    raylib::Color color = raylib::Color(red, green, blue, alpha);
    if (_font.empty())
        raylib::DrawText(_text.c_str(), getGlobalPosition().x, getGlobalPosition().y, _fontSize, color);
    else {
        raylib::Font &font = Engine::GetInstance().getResourceManager().getFont(_font);
        raylib::DrawTextEx(font, _text.c_str(), {getGlobalPosition().x, getGlobalPosition().y}, _fontSize, 0, color);
    }
}

void Label::setText(const std::string &text) { _text = text; }

void Label::setFontSize(int fontSize) { _fontSize = fontSize; }

void Label::setFont(const std::string &font)
{
    if (!font.empty()) {
        _font = Engine::GetInstance().getGamePath() / font;
        Engine::GetInstance().getResourceManager().loadResource(_font);
    }
}
