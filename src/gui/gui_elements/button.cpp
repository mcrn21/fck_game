#include "button.h"

#include "../../fck/clipping.h"

namespace fck::gui
{

Button::Button()
{
}

Button::Button(
    sf::Texture &texture,
    const sf::Color &default_color,
    const sf::IntRect &default_frame_texture_rect,
    const sf::Color &hovered_color,
    const sf::IntRect &hovered_frame_texture_rect,
    const sf::Color &pressed_color,
    const sf::IntRect &pressed_frame_texture_rect)
    : Frame{texture, default_frame_texture_rect},
      m_default_color{default_color},
      m_default_frame_texture_rect{default_frame_texture_rect},
      m_hovered_color{hovered_color},
      m_hovered_frame_texture_rect{hovered_frame_texture_rect},
      m_pressed_color{pressed_color},
      m_pressed_frame_texture_rect{pressed_frame_texture_rect}
{
}

const sf::Color &Button::getDefaultColor() const
{
    return m_default_color;
}

void Button::setDefaultColor(const sf::Color &default_color)
{
    m_default_color = default_color;
}

const sf::IntRect &Button::getDefaultFrameTextureRect() const
{
    return m_default_frame_texture_rect;
}

void Button::setDefaultFrameTextureRect(const sf::IntRect &default_frame_texture_rect)
{
    m_default_frame_texture_rect = default_frame_texture_rect;
}

const sf::Color &Button::getHoveredColor() const
{
    return m_hovered_color;
}

void Button::setHoveredColor(const sf::Color &hovered_color)
{
    m_hovered_color = hovered_color;
}

const sf::IntRect &Button::getHoveredFrameTextureRect() const
{
    return m_hovered_frame_texture_rect;
}

void Button::setHoveredFrameTextureRect(const sf::IntRect &hovered_frame_texture_rect)
{
    m_hovered_frame_texture_rect = hovered_frame_texture_rect;
}

const sf::Color &Button::getPressedColor() const
{
    return m_pressed_color;
}

void Button::setPressedColor(const sf::Color &pressed_color)
{
    m_pressed_color = pressed_color;
}

const sf::IntRect &Button::getPressedFrameTextureRect() const
{
    return m_pressed_frame_texture_rect;
}

void Button::setPressedFrameTextureRect(const sf::IntRect &pressed_frame_texture_rect)
{
    m_pressed_frame_texture_rect = pressed_frame_texture_rect;
}

sf::Text &Button::text()
{
    return m_text;
}

void Button::onHoverIn()
{
    setFrameColor(m_hovered_color);
    setFrameTextureRect(m_hovered_frame_texture_rect);
}

void Button::onHoverOut()
{
    setFrameColor(m_default_color);
    setFrameTextureRect(m_default_frame_texture_rect);
}

void Button::onMousePressed()
{
    setFrameColor(m_pressed_color);
    setFrameTextureRect(m_pressed_frame_texture_rect);
}

void Button::onMouseReleased()
{
    if (isHovered())
    {
        setFrameColor(m_hovered_color);
        setFrameTextureRect(m_hovered_frame_texture_rect);
    }
    else
    {
        setFrameColor(m_default_color);
        setFrameTextureRect(m_default_frame_texture_rect);
    }

    clicked.emit();
}

void fck::gui::Button::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    Frame::draw(target, states);

    sf::RenderStates new_state = states;
    new_state.texture = getTexture();
    new_state.transform *= getParentsTransform();

    Clipping clipping(
        target, new_state, {2.0f, 2.0f}, getLocalBounds().getSize() - sf::Vector2f{4.0f, 5.0f});

    target.draw(m_text, new_state);
}

} // namespace fck::gui
