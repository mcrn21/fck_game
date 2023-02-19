#include "button.h"
#include "common.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"

namespace fck::gui
{

Button::Button(const sf::String &string, Widget *parent) : Widget{parent}
{
    setWidgetTheme(WidgetTheme::get<Button>());
    m_text.setString(string);
}

const sf::String &Button::getString() const
{
    return m_text.getString();
}

void Button::setString(const sf::String &string)
{
    m_text.setString(string);
    updateGeometry();
}

sf::Vector2f Button::getContentSize() const
{
    const WidgetTheme &widget_theme = getWidgetTheme();

    sf::Vector2f size = m_text.getLocalBounds().getSize();

    size += {widget_theme.padding.left, widget_theme.padding.top};
    size += {widget_theme.padding.right, widget_theme.padding.bottom};

    return size;
}

void Button::onStateChanged(WidgetState state)
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    m_background.setTextureRect(widget_theme.background.texture_rects.at(state));
    m_text.setFillColor(widget_theme.text.fill_colors.at(state));
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(state));
}

void Button::onMousePressed(const sf::Vector2f &mouse_position)
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    m_background.setTextureRect(widget_theme.background.texture_rects.at(WidgetState::PRESSED));
    m_text.setFillColor(widget_theme.text.fill_colors.at(WidgetState::PRESSED));
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(WidgetState::PRESSED));
}

void Button::onMouseReleased(const sf::Vector2f &mouse_position)
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));
    clicked();
}

void Button::onKeyPressed(const sf::Event::KeyEvent &key)
{
    if (key.code == sf::Keyboard::Enter)
    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        m_background.setTextureRect(widget_theme.background.texture_rects.at(WidgetState::PRESSED));
        m_text.setFillColor(widget_theme.text.fill_colors.at(WidgetState::PRESSED));
        m_text.setOutlineColor(widget_theme.text.outline_colors.at(WidgetState::PRESSED));
    }
}

void Button::onKeyReleased(const sf::Event::KeyEvent &key)
{
    if (key.code == sf::Keyboard::Enter)
    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
        m_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
        m_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));
        clicked();
    }
}

void Button::onResized(const sf::Vector2f &size)
{
    m_background.setSize(size);
    updateGeometry();
}

void Button::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    m_text.setFont(*ResourceCache::get<sf::Font>(widget_theme.font));
    m_text.setStyle(widget_theme.text.style);
    m_text.setCharacterSize(widget_theme.text.character_size);
    m_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
    m_text.setOutlineThickness(widget_theme.text.outline_thickness);
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));

    updateGeometry();
}

void Button::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);

    const WidgetTheme &widget_theme = getWidgetTheme();
    Clipping clipping(
        target,
        new_states,
        {widget_theme.padding.left, widget_theme.padding.top},
        getLocalBounds().getSize()
            - sf::Vector2f{
                widget_theme.padding.left + widget_theme.padding.right,
                widget_theme.padding.top + widget_theme.padding.bottom});

    target.draw(m_text, new_states);

    drawChildren(target, new_states);
}

void Button::updateGeometry()
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    positionText(m_text, getSize(), widget_theme.padding, widget_theme.text.align);
}

} // namespace fck::gui
