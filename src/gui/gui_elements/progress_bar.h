#ifndef PROGRESSBAR_ICXPZBZNAIGH_H
#define PROGRESSBAR_ICXPZBZNAIGH_H

#include "frame.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class ProgressBar : public Frame
{
public:
    ProgressBar();
    ProgressBar(
        sf::Texture &texture,
        const sf::IntRect &frame_texture_rect,
        const sf::IntRect &bar_texture_rect,
        const sf::Vector2f &bar_offset = {0.0f, 0.0f});
    ~ProgressBar() = default;

    void setTexture(sf::Texture &texture) override;

    const sf::IntRect &getBarTextureRect() const;
    void setBarTextureRect(const sf::IntRect &bar_texture_rect);

    const sf::Vector2f &getBarOffset() const;
    void setBarOffset(const sf::Vector2f &bar_offset);

    void setProgress(float progress);

    sf::Text &text();

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateBarPositions();
    void updateBarTexCoords();

private:
    sf::Vertex m_bar_vertices[4];
    sf::IntRect m_bar_texture_rect;
    sf::Vector2f m_bar_offset;

    float m_progress;

    sf::Text m_text;
};

} // namespace fck::gui

#endif // PROGRESSBAR_ICXPZBZNAIGH_H
