#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Grid : public sf::Drawable
{
public:
    Grid();
    ~Grid() = default;

    const sf::Texture *getTexture() const;
    void setTexture(const sf::Texture &texture);

    const sf::Vector2f &getPosition() const;
    void setPoisition(const sf::Vector2f &position);

    const sf::Vector2f &getCellSize() const;
    void setCellSize(const sf::Vector2f &cell_size);

    const sf::Vector2i &getSize() const;
    void setSize(const sf::Vector2i &size);

    const sf::IntRect &getTextureRect() const;
    void setTextureRect(const sf::IntRect &texture_rect);

    const sf::Vector2i &getTextureCellSize() const;
    void setTextureCellSize(const sf::Vector2i &texture_cell_size);

    void setCellTexture(const sf::Vector2i &coord, int32_t index);

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updatePositions();
    void updateTexCoords();

private:
    const sf::Texture *m_texture;

    sf::Vector2f m_position;
    sf::Vector2f m_cell_size;
    sf::Vector2i m_size;

    sf::VertexArray m_vertices;

    sf::IntRect m_texture_rect;
    sf::Vector2i m_texture_cell_size;
};

} // namespace fck::gui

#endif // GRID_H
