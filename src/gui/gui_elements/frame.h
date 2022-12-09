#ifndef FRAME_EKBGLLBRCXRS_H
#define FRAME_EKBGLLBRCXRS_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace fck::gui
{

class ProgressBar;
class Minimap;

class Frame : public sf::Drawable, public sf::Transformable
{
    friend class ProgressBar;
    friend class Minimap;

public:
    Frame();
    Frame(sf::Texture &texture, const sf::IntRect &frame_texture_rect);
    ~Frame() = default;

    sf::Texture *getTexture() const;
    virtual void setTexture(sf::Texture &texture);

    const sf::Vector2f &getFrameSize() const;
    void setFrameSize(const sf::Vector2f &frame_size);

    const sf::Vector2i &getBorderSize() const;
    void setBorderSize(const sf::Vector2i &border_size);

    const sf::IntRect &getFrameTextureRect() const;
    void setFrameTextureRect(const sf::IntRect &frame_texture_rect);

    virtual sf::FloatRect getLocalBounds() const;
    virtual sf::FloatRect getGlobalBounds() const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updatePositions();
    void updateTexCoords();

private:
    sf::Texture *m_texture;

    sf::Vector2f m_frame_size;
    sf::Vector2i m_border_size;
    sf::VertexArray m_vertices;
    sf::IntRect m_frame_texture_rect;
};

} // namespace fck::gui

#endif // FRAME_EKBGLLBRCXRS_H
