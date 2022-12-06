#ifndef LEVEL_H
#define LEVEL_H

#include "fck/a_star.h"
#include "fck/b2_dynamic_tree.h"
#include "fck/entity.h"
#include "fck/sigslot.h"
#include "fck/tmx.h"
#include "fck/world.h"

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
        enum Type
        {
            UNKNOW,
            DEFAULT,
            BOSS
        };

        void enable();
        void disable();

        std::vector<Entity> entities;

        Type type = DEFAULT;
        bool opened = false;

        struct Neighbors
        {
            std::string left;
            std::string top;
            std::string right;
            std::string bottom;
        } neighbors;
    };

    Level(World *world, b2::DynamicTree<Entity> *scene_tree, PathFinder *path_finder);
    ~Level() = default;

    bool loadFromFile(const std::string &file_name);

    const Room *room(const std::string &room_name) const;
    void enableRoom(const std::string &room_name, const sf::Vector2f &player_position);

    std::vector<std::pair<sf::Vector2i, Room::Type>> roomsMinimap() const;

private:
    void loadRoom(const Tmx::Group *room_group);

public:
    Signal<const std::string &> room_enabled;

private:
    World *m_world;
    b2::DynamicTree<Entity> *m_scene_tree;
    PathFinder *m_path_finder;

    std::unique_ptr<Tmx> m_level_tmx;

    Room *m_current_room;
    std::unordered_map<std::string, Room> m_rooms;

    Entity m_player_entity;
};

} // namespace fck

#endif // LEVEL_H
