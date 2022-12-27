#include "grid.h"

#include <spdlog/spdlog.h>

namespace fck::gui
{

Grid::Grid()
{
}

const sf::Texture *Grid::getTexture() const
{
    return m_texture;
}

void Grid::setTexture(const sf::Texture &texture)
{
    m_texture = &texture;
}

const sf::Vector2f &Grid::getPosition() const
{
    return m_position;
}

void Grid::setPoisition(const sf::Vector2f &position)
{
    m_position = position;
    updatePositions();
    updateTexCoords();
}

const sf::Vector2f &Grid::getCellSize() const
{
    return m_cell_size;
}

void Grid::setCellSize(const sf::Vector2f &cell_size)
{
    m_cell_size = cell_size;
    updatePositions();
    updateTexCoords();
}

const sf::Vector2i &Grid::getSize() const
{
    return m_size;
}

void Grid::setSize(const sf::Vector2i &size)
{
    m_size = size;
    updatePositions();
    updateTexCoords();
}

const sf::IntRect &Grid::getTextureRect() const
{
    return m_texture_rect;
}

void Grid::setTextureRect(const sf::IntRect &texture_rect)
{
    m_texture_rect = texture_rect;
    updatePositions();
    updateTexCoords();
}

const sf::Vector2i &Grid::getTextureCellSize() const
{
    return m_texture_cell_size;
}

void Grid::setTextureCellSize(const sf::Vector2i &texture_cell_size)
{
    m_texture_cell_size = texture_cell_size;
    updatePositions();
    updateTexCoords();
}

void Grid::setCellTexture(const sf::Vector2i &coord, int32_t index)
{
    if (m_vertices.getVertexCount() < ((coord.x + coord.y * m_size.x) * 6))
        return;

    sf::Vertex *quad = &m_vertices[(coord.x + coord.y * m_size.x) * 6];

    if (index < 0)
    {
        quad[0].texCoords = sf::Vector2f(0, 0);
        quad[1].texCoords = sf::Vector2f(0, 0);
        quad[2].texCoords = sf::Vector2f(0, 0);
        quad[3].texCoords = sf::Vector2f(0, 0);
        quad[4].texCoords = sf::Vector2f(0, 0);
        quad[5].texCoords = sf::Vector2f(0, 0);
        quad[0].color = sf::Color::Transparent;
        quad[1].color = sf::Color::Transparent;
        quad[2].color = sf::Color::Transparent;
        quad[3].color = sf::Color::Transparent;
        quad[4].color = sf::Color::Transparent;
        quad[5].color = sf::Color::Transparent;
        return;
    }

    int32_t tu = index % (m_texture_rect.getSize().x / m_texture_cell_size.x);
    int32_t tv = index / (m_texture_rect.getSize().x / m_texture_cell_size.x);

    sf::Vector2f texture_position = sf::Vector2f{m_texture_rect.getPosition()};

    quad[0].texCoords
        = texture_position + sf::Vector2f(tu * m_texture_cell_size.x, tv * m_texture_cell_size.y);
    quad[1].texCoords = texture_position
        + sf::Vector2f((tu + 1) * m_texture_cell_size.x, tv * m_texture_cell_size.y);
    quad[2].texCoords = texture_position
        + sf::Vector2f(tu * m_texture_cell_size.x, (tv + 1) * m_texture_cell_size.y);
    quad[3].texCoords = texture_position
        + sf::Vector2f((tu + 1) * m_texture_cell_size.x, tv * m_texture_cell_size.y);
    quad[4].texCoords = texture_position
        + sf::Vector2f(tu * m_texture_cell_size.x, (tv + 1) * m_texture_cell_size.y);
    quad[5].texCoords = texture_position
        + sf::Vector2f((tu + 1) * m_texture_cell_size.x, (tv + 1) * m_texture_cell_size.y);

    quad[0].color = sf::Color::White;
    quad[1].color = sf::Color::White;
    quad[2].color = sf::Color::White;
    quad[3].color = sf::Color::White;
    quad[4].color = sf::Color::White;
    quad[5].color = sf::Color::White;
}

void Grid::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        target.draw(m_vertices, new_state);
    }
}

void Grid::updatePositions()
{
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(m_size.x * m_size.y * 6);

    for (int32_t i = 0; i < m_size.x * m_size.y; ++i)
    {
        sf::Vector2i coord = {i % m_size.x, i / m_size.x};
        sf::Vertex *quad = &m_vertices[i * 6];

        quad[0].position
            = m_position + sf::Vector2f(coord.x * m_cell_size.x, coord.y * m_cell_size.y);
        quad[1].position
            = m_position + sf::Vector2f((coord.x + 1) * m_cell_size.x, coord.y * m_cell_size.y);
        quad[2].position
            = m_position + sf::Vector2f(coord.x * m_cell_size.x, (coord.y + 1) * m_cell_size.y);
        quad[3].position
            = m_position + sf::Vector2f((coord.x + 1) * m_cell_size.x, coord.y * m_cell_size.y);
        quad[4].position
            = m_position + sf::Vector2f(coord.x * m_cell_size.x, (coord.y + 1) * m_cell_size.y);
        quad[5].position = m_position
            + sf::Vector2f((coord.x + 1) * m_cell_size.x, (coord.y + 1) * m_cell_size.y);
    }
}

void Grid::updateTexCoords()
{
}

} // namespace fck::gui
