#ifndef LEVEL_ZKKSOSIXHXVE_H
#define LEVEL_ZKKSOSIXHXVE_H

#include "fck/a_star.h"
#include "fck/b2_dynamic_tree.h"
#include "fck/entity.h"
#include "fck/sigslot.h"
#include "fck/tmx.h"
#include "fck/vector_2d.h"
#include "fck/world.h"

#include "spdlog/spdlog.h"

#include <functional>
#include <unordered_map>
#include <vector>

namespace fck
{

class Level
{
public:
    struct Room
    {
        enum Side
        {
            LEFT = 1,
            TOP = 2,
            RIGHT = 4,
            BOTTOM = 8
        };

        enum Type
        {
            UNKNOW,
            DEFAULT,
            BOSS,
            TRADER
        };

        int32_t neighbors = 0;
        Type type = DEFAULT;
        bool open = false;
    };

    Level(World *world, b2::DynamicTree<Entity> *scene_tree, PathFinder *path_finder);
    ~Level();

    bool loadFromFile(const std::string &file_name);

    void generateRoomsMap();
    void generateRoomsContent();

    const Vector2D<Room *> &getRoomsMap() const;
    const sf::Vector2i &getFirstRoomCoord() const;
    const sf::Vector2i &getCurrentRoomCoord() const;
    sf::Vector2f getRoomPixelsSize() const;

    void enableRoom(const sf::Vector2i &coord, const sf::Vector2f &target_position);

private:
    Entity createRoomTransition(Room::Side side, const sf::Vector2i &room_coord);
    std::vector<Entity> createRoom(const Tmx::Group &room_group);
    std::vector<Entity> createRoomCollisions(const Tmx::ObjectGroup &collisions_object_group);
    std::vector<Entity> createRoomEntities(const Tmx::ObjectGroup &entities_object_group);
    Entity createTileMapFromLayer(const Tmx::Layer &layer);

    const Tmx::Tileset *getTilesetByGid(int32_t gid);

public:
    Signal<const sf::Vector2i &> room_opened;
    Signal<const sf::Vector2i &> room_enabled;

private:
    World *m_world;
    b2::DynamicTree<Entity> *m_scene_tree;
    PathFinder *m_path_finder;

    std::unique_ptr<Tmx> m_level_tmx;

    struct RoomsCache
    {
        void clear()
        {
            for (int32_t i = 0; i < map.getSize(); ++i)
                delete map[i];
            map.clear();

            for (int32_t i = 0; i < entities.getSize(); ++i)
                delete entities[i];
            entities.clear();

            m_first_room_coord = {0, 0};

            spdlog::info("Rooms cleared");
        }

        void enableRoom(const sf::Vector2i &room_coord)
        {
            if (entities.getData(room_coord))
            {
                for (Entity &entity : *entities.getData(room_coord))
                    entity.enable();
            }
        }

        void disableRoom(const sf::Vector2i &room_coord)
        {
            if (entities.getData(room_coord))
            {
                for (Entity &entity : *entities.getData(room_coord))
                    entity.disable();
            }
        }

        Vector2D<Room *> map;
        Vector2D<std::vector<Entity> *> entities;
        sf::Vector2i m_first_room_coord;
    } m_rooms_cache;

    sf::Vector2i m_current_room_coord;
    Entity m_player_entity;
};

} // namespace fck

#endif // LEVEL_ZKKSOSIXHXVE_H
