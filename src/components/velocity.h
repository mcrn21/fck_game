#ifndef VELOCITY_ZUBIRPSQMWMH_H
#define VELOCITY_ZUBIRPSQMWMH_H

#include "../knowledge_base.h"

#include <SFML/System/Vector2.hpp>

#include <optional>
#include <vector>

namespace fck
{

namespace component
{

struct Velocity
{
    sf::Vector2f velocity;
    sf::Vector2f max_velocity;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Velocity> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::VELOCITY;
    }

    void init(toml::table *table)
    {
        if (table->contains("max_velocity"))
            max_velocity = vector2::tomlArrayToVector2f(table->at("max_velocity").as_array());
    }

    void create(Entity &entity)
    {
        component::Velocity &component = entity.addComponent<component::Velocity>();

        component.max_velocity = max_velocity;
    }

    sf::Vector2f max_velocity;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Velocity);

}

} // namespace fck

#endif // VELOCITY_ZUBIRPSQMWMH_H
