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

void Level::enableRoom(const std::string &room_name)
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
            return true;
        });

        m_current_room->disable();
        m_current_room = nullptr;
    }

    auto rooms_found = m_rooms.find(room_name);
    if (rooms_found == m_rooms.end())
        return;

    m_current_room = &rooms_found->second;
    m_current_room->enable();
}

void Level::loadRoom(const Tmx::Group *room_group)
{
    Room room;

    // Layers
    for (const Tmx::Layer &layer : room_group->layers)
    {
        if (layer.name.starts_with("floor") || layer.name.starts_with("wall"))
        {
            Entity entity = m_world->createEntity();

            TransformComponent &transform_component = entity.addComponent<TransformComponent>();

            SceneComponent &scene_component = entity.addComponent<SceneComponent>();
            scene_component.local_bounds
                = {{0.0f, 0.0f},
                   sf::Vector2f{vector2::mult(m_level_tmx->size(), m_level_tmx->tileSize())}};

            DrawableComponent &drawable_component = entity.addComponent<DrawableComponent>();
            TileMap *tile_map
                = TileMap::createFromTmx(m_level_tmx.get(), layer.name, room_group->name);
            drawable_component.drawable.reset(tile_map);

            drawable_component.global_bounds = sf::FloatRect(
                transform_component.transform.getPosition(), tile_map->globalBounds().getSize());

            drawable_component.z_order = std::stoi(layer.properties.at("z_order"));
            drawable_component.z_order_fill_y_coordinate = false;

            room.entities.push_back(entity);
        }
    }

    // Objects
    for (const Tmx::ObjectGroup &objects_group : room_group->object_groups)
    {
        if (objects_group.name.starts_with("collisions"))
        {
            for (const Tmx::Object &object : objects_group.objects)
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
        }

        if (objects_group.name.starts_with("room_transitions"))
        {
            for (const Tmx::Object &object : objects_group.objects)
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
        }
    }

    m_rooms.emplace(room_group->name, room);
}

} // namespace fck
