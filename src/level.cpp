#include "level.h"
#include "entity_utils.h"

#include "components/components.h"
#include "entity_scripts/entity_scripts.h"

#include "fck/event_dispatcher.h"
#include "fck/noise.h"
#include "fck/tile_map.h"

#include <queue>
#include <random>

namespace fck
{

namespace level_helpers
{

sf::IntRect gridBoundsBySceneBounds(
    const sf::FloatRect &scene_bounds, const sf::Vector2i &cell_size)
{
    sf::Vector2i lower_point = {int32_t(scene_bounds.left), int32_t(scene_bounds.top)};

    sf::Vector2i upper_point
        = {int32_t(scene_bounds.left + scene_bounds.width),
           int32_t(scene_bounds.top + scene_bounds.height)};

    sf::Vector2i lower_cell = {lower_point.x / cell_size.x, lower_point.y / cell_size.y};

    if (lower_point.x % cell_size.x > cell_size.x * 0.8)
        ++lower_cell.x;

    if (lower_point.y % cell_size.y > cell_size.y * 0.8)
        ++lower_cell.y;

    sf::Vector2i upper_cell = {upper_point.x / cell_size.x, upper_point.y / cell_size.y};

    if (upper_point.x % cell_size.x > cell_size.x * 0.2)
        ++upper_cell.x;

    if (upper_point.y % cell_size.y > cell_size.y * 0.2)
        ++upper_cell.y;

    sf::IntRect grid_bounds;

    grid_bounds.left = lower_cell.x;
    grid_bounds.top = lower_cell.y;

    grid_bounds.width = upper_cell.x - lower_cell.x;
    grid_bounds.height = upper_cell.y - lower_cell.y;

    grid_bounds = rect::extends(grid_bounds, {1, 1});

    return grid_bounds;
}

void updateCellWeight(Grid<int32_t> &grid, const sf::IntRect &bounds, bool reduce)
{
    for (int32_t i = 0; i < bounds.width; ++i)
    {
        for (int32_t j = 0; j < bounds.height; ++j)
        {
            sf::Vector2i coord = {bounds.left + i, bounds.top + j};
            if (coord.x < 0 || coord.y < 0 || coord.x >= grid.getSize2D().x
                || coord.y >= grid.getSize2D().y)
                continue;

            int32_t &tile_weight = grid.getData(coord);
            tile_weight += reduce ? -1 : 1;
        }
    }
}

} // namespace level_helpers

Room::Room() : m_neighbors{0}, m_type{chunk_type::DEFAULT}, m_open{false}
{
}

int32_t Room::getNeighbors() const
{
    return m_neighbors;
}

void Room::setNeighbors(int32_t neighbors)
{
    m_neighbors = neighbors;
}

chunk_type::Type Room::getType() const
{
    return m_type;
}

void Room::setType(chunk_type::Type type)
{
    m_type = type;
}

const std::unordered_map<chunk_side::Side, sf::Vector2f> &Room::getEntryPoints() const
{
    return m_entry_points;
}

void Room::setEntryPoints(const std::unordered_map<chunk_side::Side, sf::Vector2f> &entry_points)
{
    m_entry_points = entry_points;
}

const Grid<int32_t> &Room::getWalls() const
{
    return m_walls;
}

void Room::setWalls(const Grid<int32_t> &walls)
{
    m_walls = walls;
}

const Grid<tile_material_type::Type> &Room::getTileMaterials() const
{
    return m_tile_materials;
}

void Room::setTileMaterials(const Grid<tile_material_type::Type> &tile_materials)
{
    m_tile_materials = tile_materials;
}

bool Room::isOpen() const
{
    return m_open;
}

void Room::setOpen(bool open)
{
    m_open = open;
}

Level::Level(World *world, b2::DynamicTree<Entity> *scene_tree)
    : m_world{world},
      m_scene_tree{scene_tree},
      m_level_tmx{std::make_unique<Tmx>()},
      m_current_room_coord{-1, -1}
{
}

Level::~Level()
{
    m_rooms_cache.clear();
}

bool Level::loadFromFile(const std::string &file_name)
{
    return m_level_tmx->loadFromFile(file_name);
}

void Level::createSingleRoom(int32_t neighbors)
{
    Vector2D<BoolProxy> rooms_map;
    rooms_map.resize({1, 1}, true);
    createRoomsMap(rooms_map);

    m_rooms_cache.map[0]->setNeighbors(neighbors);
}

void Level::createRoomsMap(const Vector2D<BoolProxy> &rooms_map)
{
    m_rooms_cache.clear();

    m_rooms_cache.map.resize(rooms_map.getSize2D(), nullptr);

    for (int32_t i = 0; i < rooms_map.getSize(); ++i)
    {
        if (rooms_map.at(i))
            m_rooms_cache.map[i] = new Room{};
    }

    sf::Vector2i min_room_coord = {-1, -1};

    for (int32_t i = 0; i < m_rooms_cache.map.getSize(); ++i)
    {
        if (!m_rooms_cache.map[i])
            continue;

        auto coord = m_rooms_cache.map.transformIndex(i);

        if (min_room_coord.x == -1)
            min_room_coord = coord;

        int32_t neighbors = m_rooms_cache.map[i]->getNeighbors();

        if (coord.x - 1 >= 0 && m_rooms_cache.map.getData({coord.x - 1, coord.y}))
            neighbors |= chunk_side::LEFT;

        if (coord.x + 1 < m_rooms_cache.map.getSize2D().x
            && m_rooms_cache.map.getData({coord.x + 1, coord.y}))
            neighbors |= chunk_side::RIGHT;

        if (coord.y - 1 >= 0 && m_rooms_cache.map.getData({coord.x, coord.y - 1}))
            neighbors |= chunk_side::TOP;

        if (coord.y + 1 < m_rooms_cache.map.getSize2D().y
            && m_rooms_cache.map.getData({coord.x, coord.y + 1}))
            neighbors |= chunk_side::BOTTOM;

        m_rooms_cache.map[i]->setNeighbors(neighbors);
    }

    m_rooms_cache.m_first_room_coord = min_room_coord;
}

void Level::createRandomRoomsMap(int32_t room_count)
{
    m_rooms_cache.clear();

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

    while (coords.size() < room_count)
    {
        spdlog::info("Generate rooms");

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

        while (coords.size() < room_count && !coords_queue.empty())
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

        spdlog::info("Room count: {}", coords.size());
    }

    m_rooms_cache.map.resize(
        {map_upper_bound.x - map_lower_bound.x + 1, map_upper_bound.y - map_lower_bound.y + 1},
        nullptr);

    for (const sf::Vector2i &coord : coords)
    {
        m_rooms_cache.map.getData({coord.x - map_lower_bound.x, coord.y - map_lower_bound.y})
            = new Room();
    }

    sf::Vector2i min_room_coord = {-1, -1};

    for (int32_t i = 0; i < m_rooms_cache.map.getSize(); ++i)
    {
        if (!m_rooms_cache.map[i])
            continue;

        auto coord = m_rooms_cache.map.transformIndex(i);

        if (min_room_coord.x == -1)
            min_room_coord = coord;

        int32_t neighbors = m_rooms_cache.map[i]->getNeighbors();

        if (coord.x - 1 >= 0 && m_rooms_cache.map.getData({coord.x - 1, coord.y}))
            neighbors |= chunk_side::LEFT;

        if (coord.x + 1 < m_rooms_cache.map.getSize2D().x
            && m_rooms_cache.map.getData({coord.x + 1, coord.y}))
            neighbors |= chunk_side::RIGHT;

        if (coord.y - 1 >= 0 && m_rooms_cache.map.getData({coord.x, coord.y - 1}))
            neighbors |= chunk_side::TOP;

        if (coord.y + 1 < m_rooms_cache.map.getSize2D().y
            && m_rooms_cache.map.getData({coord.x, coord.y + 1}))
            neighbors |= chunk_side::BOTTOM;

        m_rooms_cache.map[i]->setNeighbors(neighbors);
    }

    m_rooms_cache.m_first_room_coord = min_room_coord;
}

void Level::createRoomsContent()
{
    m_rooms_cache.entities.resize(m_rooms_cache.map.getSize2D(), nullptr);

    for (int32_t i = 0; i < m_rooms_cache.entities.getSize(); ++i)
    {
        if (m_rooms_cache.map[i])
        {
            m_rooms_cache.entities[i] = new std::vector<Entity>();

            for (const auto &room_group : m_level_tmx->getGroups())
            {
                std::vector<std::string> room_group_name_strs = string::split(room_group.name, ',');
                int32_t neighbors = 0;

                for (std::string room_group_name : room_group_name_strs)
                {
                    string::trim(room_group_name);
                    neighbors |= chunk_side::fromString(room_group_name);
                }

                if (m_rooms_cache.map[i]->getNeighbors() == neighbors)
                    createRoom(i, room_group);
            }
        }
    }
}

const Vector2D<Room *> &Level::getRoomsMap() const
{
    return m_rooms_cache.map;
}

const sf::Vector2i &Level::getFirstRoomCoord() const
{
    return m_rooms_cache.m_first_room_coord;
}

const sf::Vector2i &Level::getCurrentRoomCoord() const
{
    return m_current_room_coord;
}

sf::Vector2f Level::getRoomPixelsSize() const
{
    return sf::Vector2f{vector2::mult(m_level_tmx->getSize(), m_level_tmx->getTileSize())};
}

void Level::enableRoom(const sf::Vector2i &coord, const sf::Vector2f &target_position)
{
    if (coord == m_current_room_coord)
        return;

    if (m_current_room_coord.x != -1 && m_current_room_coord.y != -1)
    {
        m_rooms_cache.entities.getData(m_current_room_coord)->clear();

        sf::FloatRect bounds
            = {sf::Vector2f{m_level_tmx->getTileSize()} * -1.0f,
               sf::Vector2f{vector2::mult(m_level_tmx->getSize(), m_level_tmx->getTileSize())}
                   + sf::Vector2f{m_level_tmx->getTileSize()} * 2.0f};
        m_scene_tree->querry(bounds, [this](int32_t proxy_id) {
            Entity entity = m_scene_tree->getUserData(proxy_id);

            if (!entity.has<component::Player>())
                m_rooms_cache.entities.getData(m_current_room_coord)->push_back(entity);
            else
                m_player_entity = entity;

            return true;
        });

        m_rooms_cache.disableRoom(m_current_room_coord);
        m_current_room_coord = {-1, -1};
    }

    // force refresh world
    m_world->refresh();

    if (m_player_entity.isValid())
    {
        entity::set_position.emit(m_player_entity, target_position);
        component::Player &player_component = m_player_entity.get<component::Player>();
        player_component.view_hard_set_position = true;
    }

    if (m_rooms_cache.entities.getData(coord))
    {
        m_rooms_cache.enableRoom(coord);
        m_current_room_coord = coord;

        if (!m_rooms_cache.map.getData(coord)->isOpen())
        {
            m_rooms_cache.map.getData(coord)->setOpen(true);
            room_opened.emit(coord);
        }

        room_enabled.emit(coord);
    }
}

void Level::createRoom(int32_t index, const Tmx::Group &rooms_group)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int32_t> dist(0, rooms_group.groups.size() - 1);

    const Tmx::Group &room_group = rooms_group.groups[dist(rng)];

    createRoomTilemapLayers(index, room_group.layers);

    for (const Tmx::ObjectGroup &object_group : room_group.object_groups)
    {
        if (object_group.name == "collisions")
            createRoomCollisions(index, object_group);

        if (object_group.name == "entities")
            createRoomEntities(index, object_group);

        if (object_group.name == "entry_points")
        {
            std::unordered_map<chunk_side::Side, sf::Vector2f> entry_points;

            for (const Tmx::Object &entry_point_object : object_group.objects)
                entry_points.emplace(
                    chunk_side::fromString(entry_point_object.name),
                    entry_point_object.rect.getPosition());

            m_rooms_cache.map[index]->setEntryPoints(entry_points);
        }

        if (object_group.name == "jump_blocks")
            createRoomJumpBlocks(index, object_group);
    }

    // Make walls grid
    Grid<int32_t> walls = Grid<int32_t>(
        m_level_tmx->getTileSize() / 2,
        vector2::mult(m_level_tmx->getTileSize(), m_level_tmx->getSize()),
        0);

    for (const Entity &entity : *m_rooms_cache.entities[index])
    {
        component::Scene &scene_component = entity.get<component::Scene>();
        if (scene_component.path_finder_wall)
        {
            sf::IntRect grid_bound = level_helpers::gridBoundsBySceneBounds(
                scene_component.global_bounds, m_level_tmx->getTileSize() / 2);
            level_helpers::updateCellWeight(walls, grid_bound, false);
        }
    }

    m_rooms_cache.map[index]->setWalls(walls);
}

void Level::createRoomTilemapLayers(int32_t index, const std::vector<Tmx::Layer> &layers)
{
    int32_t z_order = 0;
    Grid<tile_material_type::Type> tile_material_types(
        m_level_tmx->getTileSize(), m_level_tmx->getSize(), tile_material_type::NO_TYPE);

    for (int32_t i = 0; i < layers.size(); ++i)
    {
        auto entity_tilemap = createTileMapFromLayer(layers[i]);

        if (entity_tilemap.first.isValid())
        {
            component::Drawable &drawable_component
                = entity_tilemap.first.get<component::Drawable>();
            drawable_component.z_order = z_order++;

            // set tile materials
            TileMap *tile_map = static_cast<TileMap *>(drawable_component.proxy->data());
            for (int32_t j = 0; j < tile_material_types.getSize(); ++j)
            {
                if (tile_map->getTiles().at(j) > 0)
                {
                    int32_t tile_id = tile_map->getTiles().at(j);
                    auto tiles_found = std::find_if(
                        entity_tilemap.second->tiles.begin(),
                        entity_tilemap.second->tiles.end(),
                        [tile_id](const Tmx::Tile &tile) { return tile.id == tile_id; });

                    if (tiles_found != entity_tilemap.second->tiles.end())
                        tile_material_types[j] = tile_material_type::fromString(
                            tiles_found->properties.at("material"));
                }
            }

            m_rooms_cache.entities[index]->push_back(entity_tilemap.first);
        }
    }

    m_rooms_cache.map[index]->setTileMaterials(tile_material_types);
}

void Level::createRoomJumpBlocks(int32_t index, const Tmx::ObjectGroup &exit_blocks_object_group)
{
    static std::unordered_map<chunk_side::Side, sf::Vector2i> neighbor_rooms
        = {{chunk_side::LEFT, {-1, 0}},
           {chunk_side::RIGHT, {1, 0}},
           {chunk_side::TOP, {0, -1}},
           {chunk_side::BOTTOM, {0, 1}}};

    for (const Tmx::Object &exit_block_object : exit_blocks_object_group.objects)
    {
        if (exit_block_object.type != Tmx::Object::RECT)
            continue;

        chunk_side::Side side = chunk_side::fromString(exit_block_object.name);

        Entity entity = m_world->createEntity();

        component::Transform &transform_component = entity.add<component::Transform>();

        component::Scene &scene_component = entity.add<component::Scene>();
        scene_component.local_bounds
            = {{0.0f, 0.0f}, sf::Vector2f{exit_block_object.rect.getSize()}};

        component::Collision &collision_component = entity.add<component::Collision>();

        component::Script &script_component = entity.add<component::Script>();

        entity_script::RoomsJump *room_transition_script = new entity_script::RoomsJump{this};

        room_transition_script->setSide(side);

        sf::Vector2i target_room_coord
            = m_rooms_cache.map.transformIndex(index) + neighbor_rooms[side];
        room_transition_script->setRoomCoord(target_room_coord);

        script_component.entity_script.reset(room_transition_script);

        entity::set_position.emit(entity, sf::Vector2f{exit_block_object.rect.getPosition()});

        m_rooms_cache.entities[index]->push_back(entity);
    }
}

void Level::createRoomCollisions(int32_t index, const Tmx::ObjectGroup &collisions_object_group)
{
    for (const Tmx::Object &collision_object : collisions_object_group.objects)
    {
        if (collision_object.type != Tmx::Object::RECT)
            continue;

        Entity entity = m_world->createEntity();

        component::Transform &transform_component = entity.add<component::Transform>();

        component::Scene &scene_component = entity.add<component::Scene>();
        scene_component.local_bounds
            = {{0.0f, 0.0f}, sf::Vector2f{collision_object.rect.getSize()}};
        scene_component.path_finder_wall = true;

        component::Collision &collision_component = entity.add<component::Collision>();
        collision_component.wall = true;

        entity::set_position.emit(entity, sf::Vector2f{collision_object.rect.getPosition()});

        m_rooms_cache.entities[index]->push_back(entity);
    }
}

void Level::createRoomEntities(int32_t index, const Tmx::ObjectGroup &entities_object_group)
{
    for (const Tmx::Object &entity_object : entities_object_group.objects)
    {
        if (entity_object.type != Tmx::Object::RECT)
            continue;

        Entity entity = KnowledgeBase::createEntity(entity_object.name, m_world);
        if (entity.isValid())
        {
            entity::set_position.emit(
                entity,
                sf::Vector2f{entity_object.rect.getPosition()}
                    + sf::Vector2f{entity_object.rect.getSize()} / 2.0f);

            for (const auto &it : entity_object.properties)
            {
                if (it.first == "drawable_state")
                {
                    if (it.second.starts_with('['))
                    {
                        std::string states = it.second;
                        states.erase(0, 1);
                        states.erase(states.size() - 1, 1);
                        std::vector<std::string> state_strs = string::split(states, ',');

                        state_strs.erase(
                            std::remove(state_strs.begin(), state_strs.end(), ""),
                            state_strs.end());

                        if (state_strs.empty())
                            state_strs = entity::getDrawableStates(entity);

                        if (!state_strs.empty())
                        {
                            std::random_device dev;
                            std::mt19937 rng(dev());
                            std::uniform_int_distribution<int32_t> dist(0, state_strs.size() - 1);

                            entity::set_drawable_state.emit(
                                entity, string::trimCopy(state_strs[dist(rng)]));
                        }
                    }
                    else
                    {
                        entity::set_drawable_state.emit(entity, it.second);
                    }
                }
                else if (it.first == "direction")
                {
                    entity::set_direction.emit(
                        entity, entity_state::directionFromString(it.second));
                }
            }

            m_rooms_cache.entities[index]->push_back(entity);
        }
    }
}

std::pair<Entity, const Tmx::Tileset *> Level::createTileMapFromLayer(const Tmx::Layer &layer)
{
    Entity entity = m_world->createEntity();

    component::Transform &transform_component = entity.add<component::Transform>();

    component::Scene &scene_component = entity.add<component::Scene>();
    scene_component.local_bounds
        = {{0.0f, 0.0f},
           sf::Vector2f{vector2::mult(m_level_tmx->getSize(), m_level_tmx->getTileSize())}};

    component::Drawable &drawable_component = entity.add<component::Drawable>();

    int32_t gid = 0;

    for (int32_t i = 0; i < layer.tiles.size(); ++i)
    {
        if (layer.tiles[i] > 0)
        {
            gid = layer.tiles[i];
            break;
        }
    }

    const Tmx::Tileset *tileset = getTilesetByGid(gid);
    if (!tileset)
    {
        entity.destroy();
        return {};
    }

    TileMap *tile_map = TileMap::createFromTmxLayer(
        layer,
        m_level_tmx->getSize(),
        m_level_tmx->getTileSize(),
        tileset->name,
        tileset->first_gid);
    if (!tile_map)
    {
        entity.destroy();
        return {};
    }

    drawable_component.proxy.reset(new DrawableProxy(tile_map));

    drawable_component.global_bounds = sf::FloatRect(
        transform_component.transform.getPosition(), tile_map->getGlobalBounds().getSize());

    drawable_component.z_order_fill_y_coordinate = false;

    return {entity, tileset};
}

const Tmx::Tileset *Level::getTilesetByGid(int32_t gid)
{
    for (const Tmx::Tileset &tileset : m_level_tmx->getTilesets())
    {
        if (gid >= tileset.first_gid && gid < (tileset.first_gid + tileset.tile_count))
            return &tileset;
    }

    return nullptr;
}

void Level::RoomsCache::clear()
{
    for (int32_t i = 0; i < map.getSize(); ++i)
    {
        if (map[i])
            delete map[i];
    }
    map.clear();

    for (int32_t i = 0; i < entities.getSize(); ++i)
    {
        if (entities[i])
        {
            for (Entity &entity : *entities[i])
                entity.destroy();
            delete entities[i];
        }
    }
    entities.clear();

    m_first_room_coord = {0, 0};
}

void Level::RoomsCache::enableRoom(const sf::Vector2i &room_coord)
{
    if (entities.getData(room_coord))
    {
        for (Entity &entity : *entities.getData(room_coord))
            entity.enable();
    }
}

void Level::RoomsCache::disableRoom(const sf::Vector2i &room_coord)
{
    if (entities.getData(room_coord))
    {
        for (Entity &entity : *entities.getData(room_coord))
            entity.disable();
    }
}

} // namespace fck
