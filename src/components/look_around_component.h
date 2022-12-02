#ifndef LOOK_AROUND_COMPONENT_H
#define LOOK_AROUND_COMPONENT_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include "SFML/Graphics/Rect.hpp"

#include <list>

namespace fck
{

struct LookAroundComponent
{
    bool enable = true;
    float distance = 0.0f;

    sf::FloatRect global_bounds;
    sf::FloatRect global_look_bounds;

    std::list<Entity> found_entities;
    std::list<Entity> look_at_entities;
};

template<>
struct KnowledgeBase::ComponentItem<LookAroundComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::LOOK_AROUND;
    }

    void init(toml::table *table)
    {
        if (table->contains("enable"))
            enable = table->at("enable").as_boolean()->get();

        if (table->contains("distance"))
            distance = table->at("distance").as_floating_point()->get();
    }

    void create(Entity &entity)
    {
        LookAroundComponent &component = entity.addComponent<LookAroundComponent>();

        component.enable = enable;
        component.distance = distance;
    }

    bool enable = true;
    float distance = 0.0f;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(LookAroundComponent);

} // namespace fck

#endif // LOOK_AROUND_COMPONENT_H
