#ifndef SCENE_KOIXUOEYWBHW_H
#define SCENE_KOIXUOEYWBHW_H

#include "../knowledge_base.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/world.h"

#include "SFML/Graphics/Rect.hpp"

#include <cstdint>

namespace fck
{

class Entity;

namespace component
{

struct Scene
{
    sf::FloatRect local_bounds;
    sf::FloatRect global_bounds;

    // scene tree
    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;

    // path finding
    bool path_finder_wall = false;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Scene> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SCENE;
    }

    void init(toml::table *table)
    {
        if (table->contains("local_bounds"))
            local_bounds = rect::tomlArrayToFloatRect(table->at("local_bounds").as_array());

        if (table->contains("path_finder_wall"))
            path_finder_wall = table->at("path_finder_wall").as_boolean()->get();
    }

    void create(Entity &entity)
    {
        component::Scene &component = entity.add<component::Scene>();

        component.local_bounds = local_bounds;
        component.global_bounds = local_bounds;

        component.path_finder_wall = path_finder_wall;
    }

    sf::FloatRect local_bounds;

    float path_finder_wall = false;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Scene);

}

} // namespace fck

#endif // SCENECOMPONENT_KOIXUOEYWBHW_H
