#ifndef BUTTON_XNHNRTFCHWCZ_H
#define BUTTON_XNHNRTFCHWCZ_H

#include "frame.h"

#include "../../fck/sigslot.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Button : public Frame
{
public:
    Button();
    Button(
        sf::Texture &texture,
        const sf::Color &default_color,
        const sf::IntRect &default_frame_texture_rect,
        const sf::Color &hovered_color,
        const sf::IntRect &hovered_frame_texture_rect,
        const sf::Color &pressed_color,
        const sf::IntRect &pressed_frame_texture_rect);
    ~Button() = default;

    const sf::Color &getDefaultColor() const;
    void setDefaultColor(const sf::Color &default_color);

    const sf::IntRect &getDefaultFrameTextureRect() const;
    void setDefaultFrameTextureRect(const sf::IntRect &default_frame_texture_rect);

    const sf::Color &getHoveredColor() const;
    void setHoveredColor(const sf::Color &hovered_color);

    const sf::IntRect &getHoveredFrameTextureRect() const;
    void setHoveredFrameTextureRect(const sf::IntRect &hovered_frame_texture_rect);

    const sf::Color &getPressedColor() const;
    void setPressedColor(const sf::Color &pressed_color);

    const sf::IntRect &getPressedFrameTextureRect() const;
    void setPressedFrameTextureRect(const sf::IntRect &pressed_frame_texture_rect);

    sf::Text &text();

protected:
    void onHoverIn() override;
    void onHoverOut() override;

    void onMousePressed() override;
    void onMouseReleased() override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

public:
    Signal<> clicked;

private:
    sf::Text m_text;

    sf::Color m_default_color;
    sf::IntRect m_default_frame_texture_rect;

    sf::Color m_hovered_color;
    sf::IntRect m_hovered_frame_texture_rect;

    sf::Color m_pressed_color;
    sf::IntRect m_pressed_frame_texture_rect;
};

} // namespace fck::gui

#endif // BUTTON_XNHNRTFCHWCZ_H
