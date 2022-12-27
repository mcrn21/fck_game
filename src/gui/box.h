#ifndef BOX_UGKUMCIAWUNE_H
#define BOX_UGKUMCIAWUNE_H

#include "common.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Box : public sf::Drawable
{
public:
    Box();
    ~Box() = default;

    const sf::Texture *getTexture() const;
    void setTexture(const sf::Texture &texture);

    const sf::Vector2f &getPosition() const;
    void setPoisition(const sf::Vector2f &position);

    const sf::Vector2f &getSize() const;
    void setSize(const sf::Vector2f &size);

    const Sides<float> &getBorderSize() const;
    void setBorderSize(const Sides<float> &border_size);

    const sf::Color &getColor() const;
    void setColor(const sf::Color &color);

    const sf::IntRect &getTextureRect() const;
    void setTextureRect(const sf::IntRect &frame_texture_rect);

    const Sides<int32_t> &getBorderTextureSize() const;
    void setBorderTextureSize(const Sides<int32_t> &border_texture_size);

    bool containsPoint(const sf::Vector2f &point) const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updatePositions();
    void updateTexCoords();

private:
    const sf::Texture *m_texture;

    sf::Vector2f m_position;

    sf::Vector2f m_size;
    Sides<float> m_border_size;

    sf::VertexArray m_vertices;

    sf::IntRect m_texture_rect;
    Sides<int32_t> m_border_texture_size;
};

} // namespace fck::gui

#endif // BOX_H
