#include "chunk.h"

namespace fck::map
{

Chunk::Chunk() : m_neighbors{0}, m_type{chunk_type::DEFAULT}, m_open{false}
{
}

int32_t Chunk::getNeighbors() const
{
    return m_neighbors;
}

void Chunk::setNeighbors(int32_t neighbors)
{
    m_neighbors = neighbors;
}

const std::vector<Entity> &Chunk::getEntities() const
{
    return m_entities;
}

void Chunk::setEntities(const std::vector<Entity> &entities)
{
    m_entities = entities;
}

const std::unordered_map<chunk_side::Side, Entity> &Chunk::getChunkEntityEntities() const
{
    return m_chunk_entity_entities;
}

void Chunk::setChunkEntryEntities(const std::unordered_map<chunk_side::Side, Entity> &entities)
{
    m_chunk_entity_entities = entities;
}

const Vector2D<int32_t> &Chunk::getWalls() const
{
    return m_walls;
}

void Chunk::setWalls(const Vector2D<int32_t> &walls)
{
    m_walls = walls;
}

const sf::Vector2i &Chunk::getWallSize() const
{
    return m_wall_size;
}

void Chunk::setWallSize(const sf::Vector2i &wall_size)
{
    m_wall_size = wall_size;
}

const Vector2D<Tile> &Chunk::getTiles() const
{
    return m_tiles;
}

void Chunk::setTiles(const Vector2D<Tile> &tiles)
{
    m_tiles = tiles;
}

chunk_type::Type Chunk::getType() const
{
    return m_type;
}

void Chunk::setType(chunk_type::Type type)
{
    m_type = type;
}

bool Chunk::isOpen() const
{
    return m_open;
}

void Chunk::setOpen(bool open)
{
    m_open = open;
}

void Chunk::enable()
{
    for (Entity &entity : m_entities)
        entity.enable();
}

void Chunk::disable()
{
    for (Entity &entity : m_entities)
        entity.disable();
}

} // namespace fck::map
