#ifndef LOOKAROUND_CEVXIRNVTSNB_H
#define LOOKAROUND_CEVXIRNVTSNB_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include "SFML/Graphics/Rect.hpp"

#include <list>

namespace fck
{

namespace component
{

struct LookAround
{
    bool enable = true;
    float distance = 0.0f;

    sf::FloatRect global_bounds;
    sf::FloatRect global_look_bounds;

    std::list<Entity> found_entities;
    std::list<Entity> look_at_entities;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::LookAround> : ComponentItemBase
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
        component::LookAround &component = entity.addComponent<component::LookAround>();

        component.enable = enable;
        component.distance = distance;
    }

    bool enable = true;
    float distance = 0.0f;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(LookAround);

}

} // namespace fck

#endif // LOOKAROUND_CEVXIRNVTSNB_H
