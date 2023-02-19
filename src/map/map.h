#ifndef MAP_DWSXIQQZKECD_H
#define MAP_DWSXIQQZKECD_H

#include "chunk.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/vector_2d.h"
#include "../sigslot/signal.hpp"

namespace fck::map
{

class Map
{
    friend class Factory;

public:
    Map(b2::DynamicTree<Entity> *scene_tree);
    ~Map();

    const Vector2D<Chunk *> &getChunks() const;

    const sf::Vector2i &getTileSize() const;
    const sf::Vector2i &getAreaSize() const;
    const sf::Vector2i &getChunkSize() const;

    const sf::Vector2i &getFirstChunkCoords() const;
    const sf::Vector2i &getCurrentChunkCoords() const;

    const Chunk *getCurrentChunk() const;
    void setCurrentChunk(
        const sf::Vector2i &chunk_coords,
        const std::vector<Entity> &ignore_disabling_entities = std::vector<Entity>{});

    sf::Vector2i tileCoordsByPosition(const sf::Vector2f &position);

public:
    sigslot::signal<const sf::Vector2i &> chunk_opened;
    sigslot::signal<const sf::Vector2i &> chunk_changed;

private:
    b2::DynamicTree<Entity> *m_scene_tree;

    Vector2D<Chunk *> m_chunks;
    sf::Vector2i m_tile_size;
    sf::Vector2i m_area_size;
    sf::Vector2i m_chunk_size;

    sf::Vector2i m_first_chunk_coords;
    sf::Vector2i m_current_chunk_coords;
};

} // namespace fck::map

#endif // MAP_DWSXIQQZKECD_H
