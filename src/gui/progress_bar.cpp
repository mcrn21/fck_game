#include "progress_bar.h"

#include "../fck/resource_cache.h"

namespace fck::gui
{

ProgressBar::ProgressBar(Widget *parent) : Widget{parent}, m_progress{1.0f}
{
    setSelectable(false);

    setWidgetTheme(WidgetTheme::get<ProgressBar>());
}

float ProgressBar::getProgress() const
{
    return m_progress;
}

void ProgressBar::setProgress(float progress)
{
    m_progress = progress;
    updateGeometry();
}

const sf::String &ProgressBar::getString() const
{
    return m_text.getString();
}

void ProgressBar::setString(const sf::String &string)
{
    m_text.setString(string);
    updateGeometry();
}

const sf::Vector2f &ProgressBar::getTextOffset() const
{
    return m_text_offset;
}

void ProgressBar::setTextOffset(const sf::Vector2f &text_offset)
{
    m_text_offset = text_offset;
    updateGeometry();
}

void ProgressBar::onResized(const sf::Vector2f &size)
{
    updateGeometry();
}

void ProgressBar::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    m_bar.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_bar.setBorderSize(widget_theme.foreground.border_size);
    m_bar.setTextureRect(widget_theme.foreground.texture_rects.at(getState()));
    m_bar.setBorderTextureSize(widget_theme.foreground.texture_border_size);

    m_text.setFont(*ResourceCache::get<sf::Font>(widget_theme.font));
    m_text.setStyle(widget_theme.text.style);
    m_text.setCharacterSize(widget_theme.text.character_size);
    m_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
    m_text.setOutlineThickness(widget_theme.text.outline_thickness);
    m_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));

    updateGeometry();
}

void ProgressBar::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);
    target.draw(m_bar, new_states);
    target.draw(m_text, new_states);
}

void ProgressBar::updateGeometry()
{
    m_background.setSize(getSize());

    const WidgetTheme &widget_theme = getWidgetTheme();

    sf::Vector2f bar_size
        = getSize() - widget_theme.padding.getLowerPoint() - widget_theme.padding.getUpperPoint();
    bar_size.x *= m_progress;

    m_bar.setSize(bar_size);

    if (widget_theme.direction == RIGHT_TO_LEFT)
        m_bar.setPoisition(widget_theme.padding.getLowerPoint());
    else
        m_bar.setPoisition(
            {getSize().x - bar_size.x - widget_theme.padding.right, widget_theme.padding.top});

    positionText(m_text, getSize(), widget_theme.padding, widget_theme.text.align);
    m_text.setPosition(m_text.getPosition() + m_text_offset);
}

} // namespace fck::gui
