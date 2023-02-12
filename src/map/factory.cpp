#include "factory.h"

#include "../components/components.h"
#include "../entity_scripts/entity_scripts.h"
#include "../entity_utils.h"
#include "../fck/noise.h"
#include "../fck/tile_map.h"
#include "../fck/utilities.h"
#include "../fck_common.h"

#include <spdlog/spdlog.h>

#include <queue>
#include <random>

namespace fck::map
{

Factory::Factory(World *world, b2::DynamicTree<Entity> *scene_tree)
    : m_world{world}, m_scene_tree{scene_tree}
{
}

Map *Factory::generateMapFromFile(int32_t chunks_count, const std::string &file_name)
{
    Tmx tmx;

    if (!tmx.loadFromFile(file_name))
        return nullptr;

    std::unique_ptr<Map> map = std::make_unique<Map>(m_scene_tree);

    spdlog::info("Ganarate random chunks");
    generateRandomChunks(chunks_count, map.get());

    spdlog::info("Ganarate chunks content");
    generateChunksContent(map.get(), tmx);

    map->m_tile_size = tmx.getTileSize();
    map->m_area_size = vector2::mult(tmx.getTileSize(), tmx.getSize());
    map->m_chunk_size = tmx.getSize();

    // Random first chunk
    std::vector<sf::Vector2i> chunk_coords;
    for (int32_t i = 0; i < map->m_chunks.getSize(); ++i)
    {
        if (map->m_chunks.at(i))
            chunk_coords.push_back(map->m_chunks.transformIndex(i));
    }

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int32_t> seed_dist{0, int32_t(chunk_coords.size() - 1)};
    map->m_first_chunk_coords = chunk_coords[seed_dist(rng)];

    return map.release();
}

void Factory::generateRandomChunks(int32_t chunks_count, Map *map)
{
    sf::Vector2i noise_map_size = {100, 100};

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int32_t> seed_dist;

    noise::RigedMulti riged_multi;
    riged_multi.setFrequency(5);
    riged_multi.setOctaveCount(1);
    riged_multi.setLacunarity(2);
    riged_multi.setSeed(seed_dist(rng));

    noise::Map noise_map;
    noise::MapGenerator map_generator;
    map_generator.setModule(&riged_multi);
    map_generator.setMap(&noise_map);
    map_generator.setDestSize(noise_map_size.x, noise_map_size.y);
    map_generator.setBounds(0, 5, 0, 5);
    map_generator.generate();

    Vector2D<BoolProxy> filtred_noise_map;
    filtred_noise_map.resize(noise_map_size, false);

    float noise_min_limit = 0.33;
    for (int32_t i = 0; i < filtred_noise_map.getSize(); ++i)
    {
        auto coord = filtred_noise_map.transformIndex(i);
        float noise = (noise_map.getValue(coord.x, coord.y) + 1) / 2;
        filtred_noise_map[i] = noise > noise_min_limit;
    }

    sf::Vector2i map_lower_bound;
    sf::Vector2i map_upper_bound;

    std::queue<sf::Vector2i> coords_queue;
    std::vector<sf::Vector2i> coords;

    while (coords.size() < chunks_count)
    {
        coords.clear();
        sf::Vector2i first_coord;

        [&]() {
            sf::Vector2i search_size = noise_map_size / 10;

            std::uniform_int_distribution<int32_t> first_x_coord_dist(
                search_size.x, noise_map_size.x - search_size.x * 2);
            std::uniform_int_distribution<int32_t> first_y_coord_dist(
                search_size.y, noise_map_size.y - search_size.y * 2);

            first_coord = {first_x_coord_dist(rng), first_y_coord_dist(rng)};

            for (int32_t i = 0; i < (search_size.x * search_size.y); ++i)
            {
                sf::Vector2i xy = {i % search_size.x, i / search_size.x};
                if (filtred_noise_map.getData(first_coord + xy))
                    return;
            }
        }();

        filtred_noise_map.getData(first_coord) = false;
        map_lower_bound = map_upper_bound = first_coord;
        coords_queue.push(first_coord);

        while (coords.size() < chunks_count && !coords_queue.empty())
        {
            sf::Vector2i &coord = coords_queue.front();

            map_lower_bound.x = std::min(map_lower_bound.x, coord.x);
            map_lower_bound.y = std::min(map_lower_bound.y, coord.y);

            map_upper_bound.x = std::max(map_upper_bound.x, coord.x);
            map_upper_bound.y = std::max(map_upper_bound.y, coord.y);

            if (coord.x - 1 >= 0 && filtred_noise_map.getData({coord.x - 1, coord.y}))
            {
                coords_queue.push({coord.x - 1, coord.y});
                filtred_noise_map.getData({coord.x - 1, coord.y}) = false;
            }

            if (coord.x + 1 < filtred_noise_map.getSize2D().x
                && filtred_noise_map.getData({coord.x + 1, coord.y}))
            {
                coords_queue.push({coord.x - 1, coord.y});
                filtred_noise_map.getData({coord.x - 1, coord.y}) = false;
            }

            if (coord.y - 1 >= 0 && filtred_noise_map.getData({coord.x, coord.y - 1}))
            {
                coords_queue.push({coord.x, coord.y - 1});
                filtred_noise_map.getData({coord.x, coord.y - 1}) = false;
            }

            if (coord.y + 1 < filtred_noise_map.getSize2D().y
                && filtred_noise_map.getData({coord.x, coord.y + 1}))
            {
                coords_queue.push({coord.x, coord.y + 1});
                filtred_noise_map.getData({coord.x, coord.y + 1}) = false;
            }

            coords.push_back(coord);
            coords_queue.pop();
        }
    }

    map->m_chunks.resize(
        {map_upper_bound.x - map_lower_bound.x + 1, map_upper_bound.y - map_lower_bound.y + 1},
        nullptr);

    for (const sf::Vector2i &coord : coords)
    {
        map->m_chunks.getData({coord.x - map_lower_bound.x, coord.y - map_lower_bound.y})
            = new Chunk();
    }

    for (int32_t i = 0; i < map->m_chunks.getSize(); ++i)
    {
        if (!map->m_chunks[i])
            continue;

        auto coord = map->m_chunks.transformIndex(i);
        int32_t neighbors = map->m_chunks[i]->getNeighbors();

        if (coord.x - 1 >= 0 && map->m_chunks.getData({coord.x - 1, coord.y}))
            neighbors |= chunk_side::LEFT;

        if (coord.x + 1 < map->m_chunks.getSize2D().x
            && map->m_chunks.getData({coord.x + 1, coord.y}))
            neighbors |= chunk_side::RIGHT;

        if (coord.y - 1 >= 0 && map->m_chunks.getData({coord.x, coord.y - 1}))
            neighbors |= chunk_side::TOP;

        if (coord.y + 1 < map->m_chunks.getSize2D().y
            && map->m_chunks.getData({coord.x, coord.y + 1}))
            neighbors |= chunk_side::BOTTOM;

        map->m_chunks[i]->setNeighbors(neighbors);
    }
}

void Factory::generateChunksContent(Map *map, const Tmx &tmx)
{
    for (Chunk *chunk : map->m_chunks)
    {
        if (!chunk)
            continue;

        for (const auto &chunk_group : tmx.getGroups())
        {
            std::vector<std::string> chunk_group_name_strs = string::split(chunk_group.name, ',');
            int32_t neighbors = 0;

            for (std::string chunk_group_name : chunk_group_name_strs)
            {
                string::trim(chunk_group_name);
                neighbors |= chunk_side::fromString(chunk_group_name);
            }

            if (chunk->getNeighbors() == neighbors)
                createChunk(map, chunk, chunk_group, tmx);
        }
    }
}

void Factory::createChunk(Map *map, Chunk *chunk, const Tmx::Group &chunks_group, const Tmx &tmx)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int32_t> dist(0, chunks_group.groups.size() - 1);

    const Tmx::Group &chunk_group = chunks_group.groups[dist(rng)];

    std::vector<Entity> entities;
    std::unordered_map<chunk_side::Side, Entity> chunk_entry_entities;

    createChunkTilemaps(chunk, entities, chunk_group.layers, tmx);

    for (const Tmx::ObjectGroup &object_group : chunk_group.object_groups)
    {
        for (const Tmx::Object &object : object_group.objects)
        {
            std::string object_name = string::trimCopy(object.name);

            if (object_name.empty())
                continue;

            if (object_name == "wall")
            {
                Entity entity = createWallEntity(object);
                if (entity.isValid())
                    entities.push_back(entity);
                continue;
            }

            if (object_name == "entity")
            {
                Entity entity = createEntity(object);
                if (entity.isValid())
                    entities.push_back(entity);
                continue;
            }

            if (object_name == "chunk_entry_entity")
            {
                std::string side = object.properties.at("side");
                if (side.empty())
                    continue;

                Entity entity = createChunkEntryEntity(map, object);
                if (entity.isValid())
                {
                    entities.push_back(entity);
                    chunk_entry_entities.emplace(chunk_side::fromString(side), entity);
                }

                continue;
            }
        }
    }

    chunk->setWallSize(tmx.getTileSize() / 2);
    Vector2D<int32_t> walls{tmx.getSize() * 2};

    for (const Entity &entity : entities)
    {
        auto &scene_component = entity.get<component::Scene>();
        if (scene_component.path_finder_wall)
        {
            sf::IntRect grid_bound
                = wallBoundsBySceneBounds(scene_component.global_bounds, tmx.getTileSize() / 2);
            updateWallsWeight(walls, grid_bound);
        }
    }

    chunk->setWalls(walls);
    chunk->setEntities(entities);
    chunk->setChunkEntryEntities(chunk_entry_entities);
}

void Factory::createChunkTilemaps(
    Chunk *chunk,
    std::vector<Entity> &entities,
    const std::vector<Tmx::Layer> &layers,
    const Tmx &tmx)
{
    int32_t z_order = 0;

    Vector2D<Tile> tiles{tmx.getTileSize()};

    for (const Tmx::Layer &layer : layers)
    {
        auto [entity, tileset] = createTilemapEntity(layer, tmx);

        if (entity.isValid())
        {
            auto &drawable_component = entity.get<component::Drawable>();
            drawable_component.z_order = z_order++;

            // Set tile materials
            TileMap *tile_map = static_cast<TileMap *>(drawable_component.proxy->data());
            for (int32_t i = 0; i < tiles.getSize(); ++i)
            {
                int32_t tile_id = tile_map->getTiles().at(i);
                if (tile_id > 0)
                {
                    auto tiles_found = std::find_if(
                        tileset->tiles.begin(),
                        tileset->tiles.end(),
                        [tile_id](const Tmx::Tile &tile) { return tile.id == tile_id; });

                    if (tiles_found != tileset->tiles.end())
                        tiles[i].setMaterialType(
                            tile_material_type::fromString(tiles_found->properties.at("material")));
                }
            }

            entities.push_back(entity);
        }
    }

    chunk->setTiles(tiles);
}

std::pair<Entity, const Tmx::Tileset *> Factory::createTilemapEntity(
    const Tmx::Layer &layer, const Tmx &tmx)
{
    Entity entity = m_world->createEntity();

    auto &transform_component = entity.add<component::Transform>();

    auto &scene_component = entity.add<component::Scene>();
    scene_component.local_bounds
        = {{0.0f, 0.0f}, sf::Vector2f{vector2::mult(tmx.getSize(), tmx.getTileSize())}};

    auto &drawable_component = entity.add<component::Drawable>();

    int32_t gid = 0;

    for (int32_t i = 0; i < layer.tiles.size(); ++i)
    {
        if (layer.tiles[i] > 0)
        {
            gid = layer.tiles[i];
            break;
        }
    }

    const Tmx::Tileset *tileset = getTilesetByGid(gid, tmx);
    if (!tileset)
    {
        entity.destroy();
        return {Entity{}, nullptr};
    }

    TileMap *tile_map = TileMap::createFromTmxLayer(
        layer, tmx.getSize(), tmx.getTileSize(), tileset->name, tileset->first_gid);
    if (!tile_map)
    {
        entity.destroy();
        return {Entity{}, nullptr};
    }

    drawable_component.proxy.reset(new DrawableProxy(tile_map));
    drawable_component.z_order_fill_y_coordinate = false;

    return {entity, tileset};
}

const Tmx::Tileset *Factory::getTilesetByGid(int32_t gid, const Tmx &tmx)
{
    for (const Tmx::Tileset &tileset : tmx.getTilesets())
    {
        if (gid >= tileset.first_gid && gid < (tileset.first_gid + tileset.tile_count))
            return &tileset;
    }

    return nullptr;
}

Entity Factory::createEntity(const Tmx::Object &object)
{
    return Entity{};
}

Entity Factory::createWallEntity(const Tmx::Object &object)
{
    if (object.type != Tmx::Object::RECT)
        return {};

    Entity entity = m_world->createEntity();

    auto &transform_component = entity.add<component::Transform>();
    transform_component.transform.setPosition(sf::Vector2f{object.rect.getPosition()});

    auto &scene_component = entity.add<component::Scene>();
    scene_component.local_bounds = {{0.0f, 0.0f}, sf::Vector2f{object.rect.getSize()}};
    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);
    scene_component.path_finder_wall = true;

    auto &collision_component = entity.add<component::Collision>();
    collision_component.wall = true;

    return entity;
}

Entity Factory::createChunkEntryEntity(Map *map, const Tmx::Object &object)
{
    if (object.type != Tmx::Object::RECT)
        return {};

    chunk_side::Side side = chunk_side::fromString(object.properties.at("side"));

    Entity entity = m_world->createEntity();

    auto &transform_component = entity.add<component::Transform>();
    transform_component.transform.setPosition(sf::Vector2f{object.rect.getPosition()});

    auto &scene_component = entity.add<component::Scene>();
    scene_component.local_bounds = {{0.0f, 0.0f}, sf::Vector2f{object.rect.getSize()}};
    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    auto &collision_component = entity.add<component::Collision>();

    auto &script_component = entity.add<component::Script>();

    auto *chunk_entry_script = new entity_script::ChunkEntry{map, side};
    script_component.entity_script.reset(chunk_entry_script);

    return entity;
}

sf::IntRect Factory::wallBoundsBySceneBounds(
    const sf::FloatRect &scene_bounds, const sf::Vector2i &wall_size)
{
    sf::Vector2i lower_point = {int32_t(scene_bounds.left), int32_t(scene_bounds.top)};

    sf::Vector2i upper_point
        = {int32_t(scene_bounds.left + scene_bounds.width),
           int32_t(scene_bounds.top + scene_bounds.height)};

    sf::Vector2i lower_cell = {lower_point.x / wall_size.x, lower_point.y / wall_size.y};

    if (lower_point.x % wall_size.x > wall_size.x * 0.8)
        ++lower_cell.x;

    if (lower_point.y % wall_size.y > wall_size.y * 0.8)
        ++lower_cell.y;

    sf::Vector2i upper_cell = {upper_point.x / wall_size.x, upper_point.y / wall_size.y};

    if (upper_point.x % wall_size.x > wall_size.x * 0.2)
        ++upper_cell.x;

    if (upper_point.y % wall_size.y > wall_size.y * 0.2)
        ++upper_cell.y;

    sf::IntRect walls_bounds;

    walls_bounds.left = lower_cell.x;
    walls_bounds.top = lower_cell.y;

    walls_bounds.width = upper_cell.x - lower_cell.x;
    walls_bounds.height = upper_cell.y - lower_cell.y;

    walls_bounds = rect::extends(walls_bounds, {1, 1});

    return walls_bounds;
}

void Factory::updateWallsWeight(Vector2D<int32_t> &walls, const sf::IntRect &bounds)
{
    for (int32_t i = 0; i < bounds.width; ++i)
    {
        for (int32_t j = 0; j < bounds.height; ++j)
        {
            sf::Vector2i coord = {bounds.left + i, bounds.top + j};
            if (coord.x < 0 || coord.y < 0 || coord.x >= walls.getSize2D().x
                || coord.y >= walls.getSize2D().y)
                continue;

            int32_t &wall_weight = walls.getData(coord);
            ++wall_weight;
        }
    }
}

} // namespace fck::map
