#ifndef MAPFACTORY_UOHQHJIPDDDM_H
#define MAPFACTORY_UOHQHJIPDDDM_H

#include "map.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/tmx.h"
#include "../fck/vector_2d.h"
#include "../fck/world.h"

#include <string>
#include <unordered_map>

namespace fck::map
{

class Factory
{
public:
    Factory(World *world, b2::DynamicTree<Entity> *scene_tree);
    ~Factory() = default;

    Map *createMap(int32_t chunks_count, const std::string &file_name);

private:
    void createRandomChunks(int32_t chunks_count, Map *map);

    void createChunksContent(Map *map, const Tmx &tmx);
    void createChunk(Map *map, Chunk *chunk, const Tmx::Group &chunks_group, const Tmx &tmx);
    void createChunkTilemaps(
        Chunk *chunk,
        std::vector<Entity> &entities,
        const std::vector<Tmx::Layer> &layers,
        const Tmx &tmx);

    std::pair<Entity, const Tmx::Tileset *> createTilemapEntity(
        const Tmx::Layer &layer, const Tmx &tmx);
    const Tmx::Tileset *getTilesetByGid(int32_t gid, const Tmx &tmx);

    Entity createEntity(const Tmx::Object &object);
    Entity createWallEntity(const Tmx::Object &object);
    Entity createChunkEntryEntity(Map *map, const Tmx::Object &object);

    sf::IntRect wallBoundsBySceneBounds(
        const sf::FloatRect &scene_bounds, const sf::Vector2i &wall_size);
    void updateWallsWeight(Vector2D<int32_t> &walls, const sf::IntRect &bounds);

private:
    World *m_world;
    b2::DynamicTree<Entity> *m_scene_tree;
};

} // namespace fck::map

#endif // MAPFACTORY_UOHQHJIPDDDM_H
