#include "map.h"

#include "../fck/utilities.h"

namespace fck::map
{
Map::Map(b2::DynamicTree<Entity> *scene_tree)
    : m_scene_tree{scene_tree}, m_first_chunk_coords{-1, -1}, m_current_chunk_coords{-1, -1}
{
}

Map::~Map()
{
    for (int32_t i = 0; i < m_chunks.getSize(); ++i)
    {
        if (m_chunks[i])
            delete m_chunks[i];
    }
}

const Vector2D<Chunk *> &Map::getChunks() const
{
    return m_chunks;
}

const sf::Vector2i &Map::getTileSize() const
{
    return m_tile_size;
}

const sf::Vector2i &Map::getAreaSize() const
{
    return m_area_size;
}

const sf::Vector2i &Map::getChunkSize() const
{
    return m_chunk_size;
}

const sf::Vector2i &Map::getFirstChunkCoords() const
{
    return m_first_chunk_coords;
}

const sf::Vector2i &Map::getCurrentChunkCoords() const
{
    return m_current_chunk_coords;
}

const Chunk *Map::getCurrentChunk() const
{
    if (vector2::isNegotive(m_current_chunk_coords))
        return nullptr;

    return m_chunks.getData(m_current_chunk_coords);
}

void Map::setCurrentChunk(
    const sf::Vector2i &chunk_coords, const std::vector<Entity> &ignore_disabling_entities)
{
    if (m_current_chunk_coords == chunk_coords)
        return;

    if (!vector2::isNegotive(m_current_chunk_coords))
    {
        sf::FloatRect bounds
            = {sf::Vector2f{m_tile_size * -1}, sf::Vector2f{m_area_size + m_tile_size * 2}};

        std::vector<Entity> disabling_entities;

        m_scene_tree->querry(bounds, [&, this](int32_t proxy_id) {
            Entity entity = m_scene_tree->getUserData(proxy_id);

            if (std::find(
                    ignore_disabling_entities.begin(), ignore_disabling_entities.end(), entity)
                == ignore_disabling_entities.end())
                disabling_entities.push_back(entity);

            return true;
        });

        m_chunks.getData(m_current_chunk_coords)->setEntities(disabling_entities);
        m_chunks.getData(m_current_chunk_coords)->disable();
        m_current_chunk_coords = {-1, -1};
    }

    if (!vector2::isNegotive(chunk_coords) && m_chunks.getData(chunk_coords))
    {
        m_current_chunk_coords = chunk_coords;
        m_chunks.getData(m_current_chunk_coords)->enable();

        if (!m_chunks.getData(m_current_chunk_coords)->isOpen())
        {
            m_chunks.getData(m_current_chunk_coords)->setOpen(true);
            chunk_opened(m_current_chunk_coords);
        }

        chunk_changed(m_current_chunk_coords);
    }
}

sf::Vector2i Map::tileCoordsByPosition(const sf::Vector2f &position)
{
    if (m_tile_size.x == 0 || m_tile_size.y == 0)
        return {};

    return {int32_t(position.x) / m_tile_size.x, int32_t(position.y) / m_tile_size.y};
}

} // namespace fck::map
