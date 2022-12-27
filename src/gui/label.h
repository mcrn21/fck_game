#ifndef LABEL_FODIPFYOWXKN_H
#define LABEL_FODIPFYOWXKN_H

#include "widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Label : public Widget
{
public:
    Label(const sf::String &string = sf::String{}, Widget *parent = nullptr);
    ~Label() = default;

    const sf::Font *getFont() const;
    void setFont(const sf::Font &font);

    void setString(const sf::String &string);
    const sf::String &getString() const;

    void setCharacterSize(int32_t size);
    int32_t getCharacterSize() const;

    void setFillColor(const sf::Color &color);
    const sf::Color &getFillColor() const;

protected:
    void onStateChanged(WidgetState state) override;
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    sf::Text m_text;
};

} // namespace fck::gui

#endif // LABEL_H
