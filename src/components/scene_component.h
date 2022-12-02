#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "../knowledge_base.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/world.h"

#include "SFML/Graphics/Rect.hpp"

#include <cstdint>

namespace fck
{

class Entity;

struct SceneComponent
{
    sf::FloatRect local_bounds;
    sf::FloatRect global_bounds;

    // scene tree
    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;

    // path finding
    bool wall = false;
    bool single_cell = false;
    sf::IntRect path_finder_bounds;
    PathFinder *path_finder = nullptr;
};

template<>
struct KnowledgeBase::ComponentItem<SceneComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SCENE;
    }

    void init(toml::table *table)
    {
        if (table->contains("local_bounds"))
            local_bounds = rect::tomlArrayToFloatRect(table->at("local_bounds").as_array());
    }

    void create(Entity &entity)
    {
        SceneComponent &component = entity.addComponent<SceneComponent>();

        component.local_bounds = local_bounds;
        component.global_bounds = local_bounds;
    }

    sf::FloatRect local_bounds;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(SceneComponent);

} // namespace fck

#endif // SCENECOMPONENT_H
