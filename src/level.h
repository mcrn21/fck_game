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
        void enable();
        void disable();

        std::vector<Entity> entities;
    };

    Level(World *world, b2::DynamicTree<Entity> *scene_tree, PathFinder *path_finder);
    ~Level() = default;

    bool loadFromFile(const std::string &file_name);

    void enableRoom(const std::string &room_name);

private:
    void loadRoom(const Tmx::Group *room_group);

private:
    World *m_world;
    b2::DynamicTree<Entity> *m_scene_tree;
    PathFinder *m_path_finder;

    std::unique_ptr<Tmx> m_level_tmx;

    Room *m_current_room;
    std::unordered_map<std::string, Room> m_rooms;
};

} // namespace fck

#endif // LEVEL_H
