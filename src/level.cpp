#include "level.h"

#include "entity_utils.h"

#include "components/components.h"

#include "entity_scripts/entity_scripts.h"

#include "fck/event_dispatcher.h"
#include "fck/noise.h"
#include "fck/tile_map.h"
#include "fck/utilities.h"

#include <random>

namespace fck
{

//void Level::Room::enable()
//{
//    for (Entity &entity : entities)
//        entity.enable();
//}

//void Level::Room::disable()
//{
//    for (Entity &entity : entities)
//        entity.disable();
//}

Level::Level(World *world, b2::DynamicTree<Entity> *scene_tree, PathFinder *path_finder)
    : m_world{world},
      m_scene_tree{scene_tree},
      m_path_finder{path_finder},
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
    if (!m_level_tmx->loadFromFile(file_name))
        return false;

    m_path_finder->grid().clear();
    m_path_finder->grid().setCellSize(m_level_tmx->tileSize());
    m_path_finder->grid().resize(m_level_tmx->size());

    return true;
}

void Level::generateRoomsMap()
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
    for (int32_t i = 0; i < filtred_noise_map.size(); ++i)
    {
        auto coord = filtred_noise_map.transformIndex(i);
        float noise = (noise_map.value(coord.x, coord.y) + 1) / 2;
        filtred_noise_map[i] = noise > noise_min_limit;
    }

    sf::Vector2i map_lower_bound;
    sf::Vector2i map_upper_bound;

    std::queue<sf::Vector2i> coords_queue;
    std::vector<sf::Vector2i> coords;

    int32_t room_max_count = 100;

    while (coords.size() < room_max_count)
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
                if (filtred_noise_map.data(first_coord + xy))
                    return;
            }
        }();

        filtred_noise_map.data(first_coord) = false;
        map_lower_bound = map_upper_bound = first_coord;
        coords_queue.push(first_coord);

        while (coords.size() < room_max_count && !coords_queue.empty())
        {
            sf::Vector2i &coord = coords_queue.front();

            map_lower_bound.x = std::min(map_lower_bound.x, coord.x);
            map_lower_bound.y = std::min(map_lower_bound.y, coord.y);

            map_upper_bound.x = std::max(map_upper_bound.x, coord.x);
            map_upper_bound.y = std::max(map_upper_bound.y, coord.y);

            if (coord.x - 1 >= 0 && filtred_noise_map.data({coord.x - 1, coord.y}))
            {
                coords_queue.push({coord.x - 1, coord.y});
                filtred_noise_map.data({coord.x - 1, coord.y}) = false;
            }

            if (coord.x + 1 < filtred_noise_map.size2D().x
                && filtred_noise_map.data({coord.x + 1, coord.y}))
            {
                coords_queue.push({coord.x - 1, coord.y});
                filtred_noise_map.data({coord.x - 1, coord.y}) = false;
            }

            if (coord.y - 1 >= 0 && filtred_noise_map.data({coord.x, coord.y - 1}))
            {
                coords_queue.push({coord.x, coord.y - 1});
                filtred_noise_map.data({coord.x, coord.y - 1}) = false;
            }

            if (coord.y + 1 < filtred_noise_map.size2D().y
                && filtred_noise_map.data({coord.x, coord.y + 1}))
            {
                coords_queue.push({coord.x, coord.y + 1});
                filtred_noise_map.data({coord.x, coord.y + 1}) = false;
            }

            coords.push_back(coord);
            coords_queue.pop();
        }

        spdlog::info("Room count: {}", coords.size());
    }

    m_rooms_cache.clear();
    m_rooms_cache.map.resize(
        {map_upper_bound.x - map_lower_bound.x + 1, map_upper_bound.y - map_lower_bound.y + 1},
        nullptr);

    for (const sf::Vector2i &coord : coords)
    {
        m_rooms_cache.map.data({coord.x - map_lower_bound.x, coord.y - map_lower_bound.y})
            = new Room();
    }

    sf::Vector2i min_room_coord = {-1, -1};

    for (int32_t i = 0; i < m_rooms_cache.map.size(); ++i)
    {
        if (!m_rooms_cache.map[i])
            continue;

        auto coord = m_rooms_cache.map.transformIndex(i);

        if (min_room_coord.x == -1)
            min_room_coord = coord;

        if (coord.x - 1 >= 0 && m_rooms_cache.map.data({coord.x - 1, coord.y}))
            m_rooms_cache.map[i]->neighbors |= Room::LEFT;

        if (coord.x + 1 < m_rooms_cache.map.size2D().x
            && m_rooms_cache.map.data({coord.x + 1, coord.y}))
            m_rooms_cache.map[i]->neighbors |= Room::RIGHT;

        if (coord.y - 1 >= 0 && m_rooms_cache.map.data({coord.x, coord.y - 1}))
            m_rooms_cache.map[i]->neighbors |= Room::TOP;

        if (coord.y + 1 < m_rooms_cache.map.size2D().y
            && m_rooms_cache.map.data({coord.x, coord.y + 1}))
            m_rooms_cache.map[i]->neighbors |= Room::BOTTOM;
    }

    m_rooms_cache.m_first_room_coord = min_room_coord;
}

void Level::generateRoomsContent()
{
    m_rooms_cache.entities.resize(m_rooms_cache.map.size2D(), nullptr);

    for (int32_t i = 0; i < m_rooms_cache.entities.size(); ++i)
    {
        if (m_rooms_cache.map[i])
        {
            m_rooms_cache.entities[i] = new std::vector<Entity>();

            for (int32_t j = 0; j < m_level_tmx->layers().size(); ++j)
            {
                Entity entity = m_world->createEntity();

                TransformComponent &transform_component = entity.addComponent<TransformComponent>();

                SceneComponent &scene_component = entity.addComponent<SceneComponent>();
                scene_component.local_bounds
                    = {{0.0f, 0.0f},
                       sf::Vector2f{vector2::mult(m_level_tmx->size(), m_level_tmx->tileSize())}};

                DrawableComponent &drawable_component = entity.addComponent<DrawableComponent>();
                TileMap *tile_map
                    = TileMap::createFromTmx(m_level_tmx.get(), m_level_tmx->layers()[j].name);
                drawable_component.drawable.reset(tile_map);

                drawable_component.global_bounds = sf::FloatRect(
                    transform_component.transform.getPosition(),
                    tile_map->globalBounds().getSize());

                drawable_component.z_order = j;
                drawable_component.z_order_fill_y_coordinate = false;

                m_rooms_cache.entities[i]->push_back(entity);
            }

            if (m_rooms_cache.map[i]->neighbors & Room::LEFT)
                m_rooms_cache.entities[i]->push_back(
                    createRoomTransition(Room::LEFT, m_rooms_cache.map.transformIndex(i)));

            if (m_rooms_cache.map[i]->neighbors & Room::TOP)
                m_rooms_cache.entities[i]->push_back(
                    createRoomTransition(Room::TOP, m_rooms_cache.map.transformIndex(i)));

            if (m_rooms_cache.map[i]->neighbors & Room::RIGHT)
                m_rooms_cache.entities[i]->push_back(
                    createRoomTransition(Room::RIGHT, m_rooms_cache.map.transformIndex(i)));

            if (m_rooms_cache.map[i]->neighbors & Room::BOTTOM)
                m_rooms_cache.entities[i]->push_back(
                    createRoomTransition(Room::BOTTOM, m_rooms_cache.map.transformIndex(i)));
        }
    }
}

const Vector2D<Level::Room *> &Level::roomsMap() const
{
    return m_rooms_cache.map;
}

const sf::Vector2i &Level::firstRoomCoord() const
{
    return m_rooms_cache.m_first_room_coord;
}

const sf::Vector2i &Level::currentRoomCoord() const
{
    return m_current_room_coord;
}

void Level::enableRoom(const sf::Vector2i &coord, const sf::Vector2f &target_position)
{
    if (coord == m_current_room_coord)
        return;

    if (m_current_room_coord.x != -1 && m_current_room_coord.y != -1)
    {
        m_rooms_cache.entities.data(m_current_room_coord)->clear();

        sf::FloatRect bounds
            = {{-10.0f, 10.0f},
               sf::Vector2f{vector2::mult(m_level_tmx->size(), m_level_tmx->tileSize())}
                   + sf::Vector2f{20.0f, 20.0f}};
        m_scene_tree->querry(bounds, [this](int32_t proxy_id) {
            Entity entity = m_scene_tree->userData(proxy_id);

            if (!entity.hasComponent<PlayerComponent>())
                m_rooms_cache.entities.data(m_current_room_coord)->push_back(entity);
            else
                m_player_entity = entity;

            return true;
        });

        m_rooms_cache.disableRoom(m_current_room_coord);
        m_current_room_coord = {-1, -1};
    }

    EventDispatcher::runTasks(
        {[this, target_position]() {
             if (m_player_entity.isValid())
             {
                 entity::set_position.emit(m_player_entity, target_position);
                 PlayerComponent &player_component = m_player_entity.component<PlayerComponent>();
                 player_component.view_hard_set_position = true;
             }

             return true;
         },
         [this, coord]() {
             if (m_rooms_cache.entities.data(coord))
             {
                 m_rooms_cache.enableRoom(coord);
                 m_current_room_coord = coord;

                 if (!m_rooms_cache.map.data(coord)->opened)
                 {
                     m_rooms_cache.map.data(coord)->opened = true;
                     room_opened.emit(coord);
                 }

                 room_enabled.emit(coord);
             }

             return true;
         }});
}

Entity Level::createRoomTransition(Room::Side side, const sf::Vector2i &room_coord)
{
    std::unordered_map<Room::Side, sf::FloatRect> room_transtion_bounds;
    room_transtion_bounds[Room::LEFT] = {
        {0, float(m_level_tmx->tileSize().y)},
        {float(m_level_tmx->tileSize().x),
         float(m_level_tmx->size().y * m_level_tmx->tileSize().y - m_level_tmx->tileSize().y * 2)}};

    room_transtion_bounds[Room::RIGHT] = {
        {float(m_level_tmx->size().x * m_level_tmx->tileSize().x - m_level_tmx->tileSize().x),
         float(m_level_tmx->tileSize().y)},
        {float(m_level_tmx->tileSize().x),
         float(m_level_tmx->size().y * m_level_tmx->tileSize().y - m_level_tmx->tileSize().y * 2)}};

    room_transtion_bounds[Room::TOP] = {
        {float(m_level_tmx->tileSize().x), 0},
        {float(m_level_tmx->size().x * m_level_tmx->tileSize().x - m_level_tmx->tileSize().x * 2),
         float(m_level_tmx->tileSize().y)}};

    room_transtion_bounds[Room::BOTTOM] = {
        {float(m_level_tmx->tileSize().x),
         float(m_level_tmx->size().y * m_level_tmx->tileSize().y - m_level_tmx->tileSize().y)},
        {float(m_level_tmx->size().x * m_level_tmx->tileSize().x - m_level_tmx->tileSize().x * 2),
         float(m_level_tmx->tileSize().y)}};

    std::unordered_map<Room::Side, sf::Vector2f> target_positions;

    target_positions[Room::LEFT]
        = {m_level_tmx->size().x * m_level_tmx->tileSize().x - m_level_tmx->tileSize().x * 2.0f,
           m_level_tmx->size().y * m_level_tmx->tileSize().y / 2.0f};

    target_positions[Room::RIGHT] = {
        m_level_tmx->tileSize().x * 2.0f, m_level_tmx->size().y * m_level_tmx->tileSize().y / 2.0f};

    target_positions[Room::TOP]
        = {m_level_tmx->size().x * m_level_tmx->tileSize().x / 2.0f,
           m_level_tmx->size().y * m_level_tmx->tileSize().y - m_level_tmx->tileSize().y * 2.0f};

    target_positions[Room::BOTTOM] = {
        m_level_tmx->size().x * m_level_tmx->tileSize().x / 2.0f, m_level_tmx->tileSize().y * 2.0f};

    std::unordered_map<Room::Side, sf::Vector2i> neighbor_rooms;

    neighbor_rooms[Room::LEFT] = {-1, 0};
    neighbor_rooms[Room::RIGHT] = {1, 0};
    neighbor_rooms[Room::TOP] = {0, -1};
    neighbor_rooms[Room::BOTTOM] = {0, 1};

    Entity entity = m_world->createEntity();

    TransformComponent &transform_component = entity.addComponent<TransformComponent>();
    transform_component.transform.setPosition(room_transtion_bounds[side].getPosition());

    SceneComponent &scene_component = entity.addComponent<SceneComponent>();
    scene_component.local_bounds = sf::FloatRect{{0, 0}, room_transtion_bounds[side].getSize()};
    scene_component.path_finder_wall = true;

    CollisionComponent &collision_component = entity.addComponent<CollisionComponent>();
    ScriptComponent &script_component = entity.addComponent<ScriptComponent>();

    RoomTransitionScript *room_transition_script = new RoomTransitionScript{this};

    sf::Vector2i target_room_coord = room_coord + neighbor_rooms[side];
    room_transition_script->setRoomCoord(target_room_coord);

    room_transition_script->setTargetPosition(target_positions[side]);

    script_component.entity_script.reset(room_transition_script);

    return entity;
}

} // namespace fck
