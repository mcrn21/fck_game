#include "tile_map.h"
#include "resource_cache.h"

namespace fck
{

std::vector<TileMap *> TileMap::createFromTmx(Tmx *tmx, int32_t layer_id)
{
    std::vector<TileMap *> tilemaps;

    if (!tmx || tmx->tilesets().empty() || tmx->layers().empty())
        return tilemaps;

    for (const auto &layer : tmx->layers())
    {
        if (layer_id < 0 || layer_id == layer.id)
        {
            auto resource_image_found = layer.properties.find("resource_image");
            if (resource_image_found == layer.properties.end())
                return tilemaps;

            std::string resource_image = resource_image_found->second;

            sf::Texture *texture = ResourceCache::resource<sf::Texture>(resource_image);

            if (!texture)
                continue;

            sf::Vector2i map_size = tmx->size();
            sf::Vector2i tile_size = tmx->tileSize();

            int32_t first_gid = 1;
            for (const Tmx::Tileset &tileset : tmx->tilesets())
            {
                if (tileset.name == resource_image)
                {
                    first_gid = tileset.first_gid;
                    break;
                }
            }

            std::vector<std::vector<int32_t>> tiles(map_size.y, std::vector<int32_t>(map_size.x));

            for (int32_t i = 0; i < map_size.y; ++i)
            {
                for (int32_t j = 0; j < map_size.x; ++j)
                {
                    tiles[i][j] = layer.tiles[j][i] - first_gid;
                }
            }

            tilemaps.push_back(new TileMap{texture, map_size, tile_size, tiles});
        }
    }

    return tilemaps;
}

TileMap::TileMap() : m_texture{nullptr}
{
}

TileMap::TileMap(
    sf::Texture *texture,
    const sf::Vector2i &map_size,
    const sf::Vector2i &tile_size,
    const std::vector<std::vector<int32_t>> &tiles)
    : m_texture{texture}, m_map_size{map_size}, m_tile_size{tile_size}, m_tiles{tiles}
{
    m_tiles.resize(m_map_size.y, std::vector<int32_t>(m_map_size.x));
    updatePositions();
    updateTexCoords();
}

sf::Texture *TileMap::texture() const
{
    return m_texture;
}

void TileMap::setTexture(sf::Texture *texture, const sf::Vector2i &tile_size)
{
    if (texture && (texture != m_texture || m_tile_size != tile_size))
    {
        updateTexCoords();
    }
    m_texture = texture;
}

const sf::Color &TileMap::color() const
{
    return m_vertices[0].color;
}

void TileMap::setColor(const sf::Color &color)
{
    for (int32_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = color;
}

sf::Vector2i TileMap::mapSize() const
{
    return m_map_size;
}

void TileMap::setMapSize(const sf::Vector2i &map_size)
{
    m_map_size = map_size;
    m_tiles.clear();
    m_tiles.resize(m_map_size.y, std::vector<int32_t>(m_map_size.x));
    updatePositions();
}

sf::Vector2i TileMap::tileSize() const
{
    return m_tile_size;
}

void TileMap::setTileSize(const sf::Vector2i &tile_size)
{
    m_tile_size = tile_size;
    updatePositions();
    updateTexCoords();
}

void TileMap::setTile(const sf::Vector2i &position, int32_t tile)
{
    if (position.y >= m_tile_size.y || position.x >= m_tile_size.x)
        return;
    m_tiles[position.y][position.x] = tile;
    updateTexCoords();
}

void TileMap::setTiles(const std::vector<std::vector<int32_t>> &tiles)
{
    for (int32_t i = 0; i < tiles.size(); ++i)
    {
        if (i < m_tiles.size())
        {
            for (int32_t j = 0; j < tiles[i].size(); ++j)
            {
                if (j < m_tiles[i].size())

                    m_tiles[i][j] = tiles[i][j];
            }
        }
    }
    updateTexCoords();
}

sf::FloatRect TileMap::localBounds() const
{
    return m_vertices.getBounds();
}

sf::FloatRect TileMap::contentBounds() const
{
    return m_vertices.getBounds();
}

sf::Vector2f TileMap::center() const
{
    sf::FloatRect rect = localBounds();
    return {rect.width / 2, rect.height / 2};
}

void TileMap::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!isVisible())
        return;

    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        target.draw(m_vertices, new_state);
    }
}

void TileMap::updatePositions()
{
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(m_map_size.x * m_map_size.y * 6);

    for (int32_t i = 0; i < m_map_size.x; ++i)
    {
        for (int32_t j = 0; j < m_map_size.y; ++j)
        {
            sf::Vertex *quad = &m_vertices[(i + j * m_map_size.x) * 6];

            quad[0].position = sf::Vector2f(i * m_tile_size.x, j * m_tile_size.y);

            quad[1].position = sf::Vector2f((i + 1) * m_tile_size.x, j * m_tile_size.y);

            quad[2].position = sf::Vector2f(i * m_tile_size.x, (j + 1) * m_tile_size.y);

            quad[3].position = sf::Vector2f((i + 1) * m_tile_size.x, j * m_tile_size.y);

            quad[4].position = sf::Vector2f(i * m_tile_size.x, (j + 1) * m_tile_size.y);

            quad[5].position = sf::Vector2f((i + 1) * m_tile_size.x, (j + 1) * m_tile_size.y);
        }
    }
}

void TileMap::updateTexCoords()
{
    for (int32_t i = 0; i < m_map_size.x; ++i)
    {
        for (int32_t j = 0; j < m_map_size.y; ++j)
        {
            sf::Vertex *quad = &m_vertices[(i + j * m_map_size.x) * 6];

            if (i < m_tiles.size() && j < m_tiles[i].size())
            {
                int32_t tile_number = m_tiles[i][j];

                if (tile_number < 0)
                {
                    quad[0].texCoords = sf::Vector2f(0, 0);
                    quad[1].texCoords = sf::Vector2f(0, 0);
                    quad[2].texCoords = sf::Vector2f(0, 0);
                    quad[3].texCoords = sf::Vector2f(0, 0);
                    quad[4].texCoords = sf::Vector2f(0, 0);
                    quad[5].texCoords = sf::Vector2f(0, 0);
                    continue;
                }

                int32_t tu = tile_number % (m_texture->getSize().x / m_tile_size.x);
                int32_t tv = tile_number / (m_texture->getSize().x / m_tile_size.x);

                quad[0].texCoords = sf::Vector2f(tu * m_tile_size.x, tv * m_tile_size.y);

                quad[1].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);

                quad[2].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);

                quad[3].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);

                quad[4].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);

                quad[5].texCoords
                    = sf::Vector2f((tu + 1) * m_tile_size.x, (tv + 1) * m_tile_size.y);
            }
        }
    }
}

} // namespace fck
