#include "level.h"

#include "entity_utils.h"

#include "components/components.h"

#include "entity_scripts/entity_scripts.h"

#include "fck/event_dispatcher.h"
#include "fck/tile_map.h"

#include "spdlog/spdlog.h"

namespace fck
{

void Level::Room::enable()
{
    for (Entity &entity : entities)
        entity.enable();
}

void Level::Room::disable()
{
    for (Entity &entity : entities)
        entity.disable();
}

Level::Level(World *world, b2::DynamicTree<Entity> *scene_tree, PathFinder *path_finder)
    : m_world{world},
      m_scene_tree{scene_tree},
      m_path_finder{path_finder},
      m_level_tmx{std::make_unique<Tmx>()},
      m_current_room{nullptr}
{
}

bool Level::loadFromFile(const std::string &file_name)
{
    if (!m_level_tmx->loadFromFile(file_name))
        return false;

    m_path_finder->grid().clear();
    m_path_finder->grid().setCellSize(m_level_tmx->tileSize());
    m_path_finder->grid().resize(m_level_tmx->size());

    for (const Tmx::Group &group : m_level_tmx->groups())
        loadRoom(&group);

    return true;
}

const Level::Room *Level::room(const std::string &room_name) const
{
    auto rooms_found = m_rooms.find(room_name);
    if (rooms_found == m_rooms.end())
        return nullptr;
    return &rooms_found->second;
}

void Level::enableRoom(const std::string &room_name, const sf::Vector2f &player_position)
{
    if (m_current_room)
    {
        m_current_room->entities.clear();
        sf::FloatRect bounds
            = {{-10.0f, 10.0f},
               sf::Vector2f{vector2::mult(m_level_tmx->size(), m_level_tmx->tileSize())}
                   + sf::Vector2f{20.0f, 20.0f}};
        m_scene_tree->querry(bounds, [this](int32_t proxy_id) {
            Entity entity = m_scene_tree->userData(proxy_id);

            if (!entity.hasComponent<PlayerComponent>())
                m_current_room->entities.push_back(entity);
            else
                m_player_entity = entity;

            return true;
        });

        m_current_room->disable();
        m_current_room = nullptr;
    }

    EventDispatcher::runTasks(
        {[this, player_position]() {
             if (m_player_entity.isValid())
             {
                 entity::set_position.emit(m_player_entity, player_position);
                 PlayerComponent &player_component = m_player_entity.component<PlayerComponent>();
                 player_component.view_hard_set_position = true;
             }

             return true;
         },
         [this, room_name]() {
             auto rooms_found = m_rooms.find(room_name);
             if (rooms_found == m_rooms.end())
                 return false;

             m_current_room = &rooms_found->second;
             m_current_room->enable();

             m_current_room->opened = true;

             room_enabled.emit(room_name);

             return true;
         }});
}

std::vector<std::pair<sf::Vector2i, Level::Room::Type>> Level::roomsMinimap() const
{
    if (!m_current_room)
        return {};

    std::queue<std::pair<sf::Vector2i, const Room *>> need_check_rooms;
    need_check_rooms.push({{0, 0}, m_current_room});

    std::vector<std::pair<sf::Vector2i, Level::Room::Type>> rooms;
    std::vector<const Room *> checked_rooms;

    while (!need_check_rooms.empty())
    {
        const auto &r = need_check_rooms.front();

        rooms.push_back({r.first, r.second->opened ? r.second->type : Room::UNKNOW});

        if (r.second->opened)
        {
            if (!r.second->neighbors.left.empty())
            {
                const Room *left_room = room(r.second->neighbors.left);
                if (left_room
                    && std::find(checked_rooms.begin(), checked_rooms.end(), left_room)
                        == checked_rooms.end())
                    need_check_rooms.push({r.first - sf::Vector2i{1, 0}, left_room});
            }

            if (!r.second->neighbors.top.empty())
            {
                const Room *top_room = room(r.second->neighbors.top);
                if (top_room
                    && std::find(checked_rooms.begin(), checked_rooms.end(), top_room)
                        == checked_rooms.end())
                    need_check_rooms.push({r.first - sf::Vector2i{0, 1}, top_room});
            }

            if (!r.second->neighbors.right.empty())
            {
                const Room *right_room = room(r.second->neighbors.right);
                if (right_room
                    && std::find(checked_rooms.begin(), checked_rooms.end(), right_room)
                        == checked_rooms.end())
                    need_check_rooms.push({r.first + sf::Vector2i{1, 0}, right_room});
            }

            if (!r.second->neighbors.bottom.empty())
            {
                const Room *bottom_room = room(r.second->neighbors.bottom);
                if (bottom_room
                    && std::find(checked_rooms.begin(), checked_rooms.end(), bottom_room)
                        == checked_rooms.end())
                    need_check_rooms.push({r.first + sf::Vector2i{0, 1}, bottom_room});
            }
        }

        checked_rooms.push_back(r.second);
        need_check_rooms.pop();
    }

    return rooms;
}

void Level::loadRoom(const Tmx::Group *room_group)
{
    Room room;

    // Neighbors
    for (const auto &property : room_group->properties)
    {
        if (property.first == "neighbor_left")
            room.neighbors.left = property.second;
        else if (property.first == "neighbor_top")
            room.neighbors.top = property.second;
        else if (property.first == "neighbor_right")
            room.neighbors.right = property.second;
        else if (property.first == "neighbor_bottom")
            room.neighbors.bottom = property.second;
    }

    // layers
    for (int32_t i = 0; i < room_group->layers.size(); ++i)
    {
        Entity entity = m_world->createEntity();

        TransformComponent &transform_component = entity.addComponent<TransformComponent>();

        SceneComponent &scene_component = entity.addComponent<SceneComponent>();
        scene_component.local_bounds
            = {{0.0f, 0.0f},
               sf::Vector2f{vector2::mult(m_level_tmx->size(), m_level_tmx->tileSize())}};

        DrawableComponent &drawable_component = entity.addComponent<DrawableComponent>();
        TileMap *tile_map = TileMap::createFromTmx(
            m_level_tmx.get(), room_group->layers[i].name, room_group->name);
        drawable_component.drawable.reset(tile_map);

        drawable_component.global_bounds = sf::FloatRect(
            transform_component.transform.getPosition(), tile_map->globalBounds().getSize());

        drawable_component.z_order = i;
        drawable_component.z_order_fill_y_coordinate = false;

        room.entities.push_back(entity);
    }

    // Objects
    for (const Tmx::ObjectGroup &objects_group : room_group->object_groups)
    {
        for (const Tmx::Object &object : objects_group.objects)
        {
            auto property_found = object.properties.find("type");
            if (property_found == object.properties.end())
                continue;

            std::string object_type = property_found->second;

            if (object_type.starts_with("collision_block"))
            {
                Entity entity = m_world->createEntity();

                TransformComponent &transform_component = entity.addComponent<TransformComponent>();
                transform_component.transform.setPosition(sf::Vector2f(object.rect.getPosition()));

                SceneComponent &scene_component = entity.addComponent<SceneComponent>();
                scene_component.local_bounds = sf::FloatRect(object.rect);
                scene_component.local_bounds.left = 0;
                scene_component.local_bounds.top = 0;
                scene_component.path_finder_wall = true;

                CollisionComponent &collision_component = entity.addComponent<CollisionComponent>();
                collision_component.wall = true;

                room.entities.push_back(entity);
            }
            else if (object_type.starts_with("room_transition"))
            {
                Entity entity = m_world->createEntity();

                TransformComponent &transform_component = entity.addComponent<TransformComponent>();
                transform_component.transform.setPosition(sf::Vector2f(object.rect.getPosition()));

                SceneComponent &scene_component = entity.addComponent<SceneComponent>();
                scene_component.local_bounds = sf::FloatRect(object.rect);
                scene_component.local_bounds.left = 0;
                scene_component.local_bounds.top = 0;
                scene_component.path_finder_wall = true;

                CollisionComponent &collision_component = entity.addComponent<CollisionComponent>();

                ScriptComponent &script_component = entity.addComponent<ScriptComponent>();

                RoomTransitionScript *room_transition_script = new RoomTransitionScript{this};

                room_transition_script->setRoom(object.properties.at("room"));
                room_transition_script->setPoint(
                    vector2::stringToVector2f(object.properties.at("point"), ','));

                script_component.entity_script.reset(room_transition_script);

                room.entities.push_back(entity);
            }
            else if (object_type.starts_with("entity"))
            {
                Entity entity = KnowledgeBase::createEntity(object.properties.at("name"), m_world);
                entity::set_position.emit(entity, sf::Vector2f{object.rect.getPosition()});
                room.entities.push_back(entity);
            }
        }
    }

    m_rooms.emplace(room_group->name, room);
}

} // namespace fck
