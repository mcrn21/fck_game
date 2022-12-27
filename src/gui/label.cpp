#include "label.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"

namespace fck::gui
{

Label::Label(const sf::String &string, Widget *parent) : Widget{parent}
{
    setWidgetTheme(WidgetTheme::get<Label>());
    setString(string);
    setSelectable(false);
}

const sf::Font *Label::getFont() const
{
    return m_text.getFont();
}

void Label::setFont(const sf::Font &font)
{
    m_text.setFont(font);
    updateGeometry();
}

void Label::setString(const sf::String &string)
{
    m_text.setString(string);
    updateGeometry();
}

const sf::String &Label::getString() const
{
    return m_text.getString();
}

void Label::setCharacterSize(int32_t size)
{
    m_text.setCharacterSize(size);
    updateGeometry();
}

int32_t Label::getCharacterSize() const
{
    return m_text.getCharacterSize();
}

void Label::setFillColor(const sf::Color &color)
{
    m_text.setFillColor(color);
}

const sf::Color &Label::getFillColor() const
{
    return m_text.getFillColor();
}

void Label::onStateChanged(WidgetState state)
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    m_text.setFillColor(widget_theme.text.fill_colors.at(state));
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(state));
}

void Label::onResized(const sf::Vector2f &size)
{
    updateGeometry();
}

void Label::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_text.setFont(*ResourceCache::get<sf::Font>(widget_theme.font));
    m_text.setStyle(widget_theme.text.style);
    m_text.setCharacterSize(widget_theme.text.character_size);
    m_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
    m_text.setOutlineThickness(widget_theme.text.outline_thickness);
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));

    updateGeometry();
}

void Label::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    const WidgetTheme &widget_theme = getWidgetTheme();
    Clipping clipping(
        target,
        new_states,
        {widget_theme.padding.left, widget_theme.padding.top},
        getLocalBounds().getSize()
            - sf::Vector2f{
                widget_theme.padding.left + widget_theme.padding.right,
                widget_theme.padding.top + widget_theme.padding.bottom});

    new_states.transform *= getTransform();
    target.draw(m_text, new_states);

    drawChildren(target, new_states);
}

void Label::updateGeometry()
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    positionText(m_text, getSize(), widget_theme.padding, widget_theme.text.align);
}

} // namespace fck::gui
