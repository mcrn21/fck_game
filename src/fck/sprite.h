#ifndef SPRITE_H
#define SPRITE_H

#include "drawable.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace fck
{

class Sprite : public Drawable
{
public:
    Sprite();
    Sprite(sf::Texture *texture);
    Sprite(sf::Texture *texture, const sf::IntRect &rectangle);
    ~Sprite() = default;

    drawable_type::Type type() const;

    sf::Texture *texture() const;
    void setTexture(sf::Texture *texture, bool resetRect = false);

    const sf::IntRect &textureRect() const;
    void setTextureRect(const sf::IntRect &rectangle);

    const sf::Color &color() const;
    void setColor(const sf::Color &color);

    sf::FloatRect localBounds() const;
    sf::FloatRect globalBounds() const;

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const;

private:
    void updatePositions();
    void updateTexCoords();

private:
    sf::VertexArray m_vertices; //!< Vertices defining the sprite's geometry
    sf::Texture *m_texture; //!< Texture of the sprite
    sf::IntRect m_texture_rect; //!< Rectangle defining the area of the source texture to display
};

} // namespace fck

#endif // SPRITE_H
