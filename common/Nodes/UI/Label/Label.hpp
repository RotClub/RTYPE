//
// Created by Yanis Moyon on 14/01/2025.
//

#ifndef LABEL_HPP
#define LABEL_HPP

#include <Nodes/UI/Ui.hpp>

class Label : public UI {
public:
    explicit Label(const std::string &name = "label", const Types::Vector2 &position = {0, 0}, const std::string &text = "Label", const std::string &font = nullptr, int fontSize = 20);
    ~Label();

    void Draw() override;

    [[nodiscard]] const std::string &getText() const { return _text; }
    [[nodiscard]] int getFontSize() const { return _fontSize; }
    [[nodiscard]] const std::string &getFont() const { return _font; }

    void setText(const std::string &text);
    void setFontSize(int fontSize);
    void setFont(const std::string &font);
private:
    std::string _text;
    int _fontSize;
    std::string _font;
};



#endif //LABEL_HPP
