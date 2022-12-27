#include "tile_map.h"
#include "resource_cache.h"

namespace fck
{

TileMap *TileMap::createFromTmxLayer(
    const Tmx::Layer &layer,
    const sf::Vector2i &map_size,
    const sf::Vector2i &tile_size,
    const std::string &texture_name,
    int32_t first_gid)
{
    sf::Texture *texture = ResourceCache::get<sf::Texture>(texture_name);
    if (!texture)
        return nullptr;

    Vector2D<int32_t> tiles;
    tiles.resize(map_size);

    for (int32_t i = 0; i < layer.tiles.size(); ++i)
        tiles[i] = layer.tiles[i] - first_gid;

    return new TileMap{*texture, tile_size, tiles};
}

TileMap::TileMap() : m_texture{nullptr}
{
}

TileMap::TileMap(
    sf::Texture &texture, const sf::Vector2i &tile_size, const Vector2D<int32_t> &tiles)
    : m_texture{&texture}, m_tile_size{tile_size}, m_tiles{tiles}
{
    updatePositions();
    updateTexCoords();
}

sf::Texture *TileMap::getTexture() const
{
    return m_texture;
}

void TileMap::setTexture(sf::Texture &texture, const sf::Vector2i &tile_size)
{
    m_texture = &texture;
    setTileSize(tile_size);
}

const sf::Color &TileMap::getColor() const
{
    return m_vertices[0].color;
}

void TileMap::setColor(const sf::Color &color)
{
    for (int32_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = color;
}

const sf::Vector2i &TileMap::getMapSize() const
{
    return m_tiles.getSize2D();
}

void TileMap::setMapSize(const sf::Vector2i &map_size)
{
    m_tiles.resize(map_size);
    updatePositions();
}

sf::Vector2i TileMap::getTileSize() const
{
    return m_tile_size;
}

void TileMap::setTileSize(const sf::Vector2i &tile_size)
{
    m_tile_size = tile_size;
    updatePositions();
    updateTexCoords();
}

int32_t TileMap::getTile(const sf::Vector2i &position) const
{
    return m_tiles.getData(position);
}

const Vector2D<int32_t> &TileMap::getTiles() const
{
    return m_tiles;
}

void TileMap::setTile(const sf::Vector2i &position, int32_t tile)
{
    m_tiles.getData(position) = tile;
    updateTexCoords();
}

void TileMap::setTiles(const Vector2D<int32_t> &tiles)
{
    m_tiles = tiles;
    updatePositions();
    updateTexCoords();
}

sf::FloatRect TileMap::getLocalBounds() const
{
    return m_vertices.getBounds();
}

sf::FloatRect TileMap::getGlobalBounds() const
{
    return getTransform().transformRect(m_vertices.getBounds());
}

void TileMap::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        new_state.transform *= getTransform();
        target.draw(m_vertices, new_state);
    }
}

void TileMap::updatePositions()
{
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(m_tiles.getSize() * 6);

    for (int32_t i = 0; i < m_tiles.getSize(); ++i)
    {
        auto coord = m_tiles.transformIndex(i);
        sf::Vertex *quad = &m_vertices[i * 6];

        quad[0].position = sf::Vector2f(coord.x * m_tile_size.x, coord.y * m_tile_size.y);
        quad[1].position = sf::Vector2f((coord.x + 1) * m_tile_size.x, coord.y * m_tile_size.y);
        quad[2].position = sf::Vector2f(coord.x * m_tile_size.x, (coord.y + 1) * m_tile_size.y);
        quad[3].position = sf::Vector2f((coord.x + 1) * m_tile_size.x, coord.y * m_tile_size.y);
        quad[4].position = sf::Vector2f(coord.x * m_tile_size.x, (coord.y + 1) * m_tile_size.y);
        quad[5].position
            = sf::Vector2f((coord.x + 1) * m_tile_size.x, (coord.y + 1) * m_tile_size.y);
    }
}

void TileMap::updateTexCoords()
{
    for (int32_t i = 0; i < m_tiles.getSize(); ++i)
    {
        int32_t tile = m_tiles.at(i);
        auto coord = m_tiles.transformIndex(i);
        sf::Vertex *quad = &m_vertices[i * 6];

        if (tile < 0)
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
            continue;
        }

        int32_t tu = tile % (m_texture->getSize().x / m_tile_size.x);
        int32_t tv = tile / (m_texture->getSize().x / m_tile_size.x);

        quad[0].texCoords = sf::Vector2f(tu * m_tile_size.x, tv * m_tile_size.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);
        quad[2].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);
        quad[3].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);
        quad[4].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);
        quad[5].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, (tv + 1) * m_tile_size.y);
    }
}

} // namespace fck
