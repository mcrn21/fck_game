#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

#include <SFML/Graphics.hpp>

namespace fck
{

class ProgressBar : public sf::Drawable, public sf::Transformable
{
public:
    ProgressBar();
    ProgressBar(
        sf::Texture &texture,
        const sf::IntRect &background_texture_rect,
        const sf::IntRect &bar_texture_rect,
        const sf::Vector2f &bar_position);
    ~ProgressBar() = default;

    void setTexture(sf::Texture &texture);
    void setBackgroundTextureRect(const sf::IntRect &background_texture_rect);
    void setBarTextureRect(const sf::IntRect &bar_texture_rect);
    void setBarPosition(const sf::Vector2f &bar_position);

    void setProgress(float progress);

    sf::Text &text();

    sf::FloatRect localBounds() const;
    sf::FloatRect globalBounds() const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateBackgroundPositions();
    void updateBackgroundTexCoords();

    void updateBarPositions();
    void updateBarTexCoords();

private:
    sf::Texture *m_texture;

    sf::Vertex m_background_vertices[4];
    sf::IntRect m_background_texture_rect;

    sf::Vertex m_bar_vertices[4];
    sf::IntRect m_bar_texture_rect;
    sf::Vector2f m_bar_position;

    float m_progress;

    sf::Text m_text;
};

} // namespace fck

#endif // GUIELEMENTS_H
