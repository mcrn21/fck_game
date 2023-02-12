#ifndef CHUNK_FETGGAULVDKL_H
#define CHUNK_FETGGAULVDKL_H

#include "tile.h"

#include "../fck_common.h"

#include "../fck/entity.h"
#include "../fck/vector_2d.h"

namespace fck::map
{

class Chunk
{
public:
    Chunk();
    ~Chunk() = default;

    int32_t getNeighbors() const;
    void setNeighbors(int32_t neighbors);

    const std::vector<Entity> &getEntities() const;
    void setEntities(const std::vector<Entity> &entities);

    const std::unordered_map<chunk_side::Side, Entity> &getChunkEntityEntities() const;
    void setChunkEntryEntities(const std::unordered_map<chunk_side::Side, Entity> &entities);

    const Vector2D<int32_t> &getWalls() const;
    void setWalls(const Vector2D<int32_t> &walls);

    const sf::Vector2i &getWallSize() const;
    void setWallSize(const sf::Vector2i &wall_size);

    const Vector2D<Tile> &getTiles() const;
    void setTiles(const Vector2D<Tile> &tiles);

    chunk_type::Type getType() const;
    void setType(chunk_type::Type type);

    bool isOpen() const;
    void setOpen(bool open);

    void enable();
    void disable();

private:
    int32_t m_neighbors;
    std::vector<Entity> m_entities;
    std::unordered_map<chunk_side::Side, Entity> m_chunk_entity_entities;
    Vector2D<int32_t> m_walls;
    sf::Vector2i m_wall_size;
    Vector2D<Tile> m_tiles;
    chunk_type::Type m_type;
    bool m_open;
};

} // namespace fck::map

#endif // CHUNK_FETGGAULVDKL_H
