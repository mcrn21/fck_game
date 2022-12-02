#ifndef VELOCITYCOMPONENT_H
#define VELOCITYCOMPONENT_H

#include "../knowledge_base.h"

#include <SFML/System/Vector2.hpp>

#include <optional>
#include <vector>

namespace fck
{

struct VelocityComponent
{
    sf::Vector2f velocity;
    sf::Vector2f max_velocity;
};

template<>
struct KnowledgeBase::ComponentItem<VelocityComponent> : ComponentItemBase
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
        VelocityComponent &component = entity.addComponent<VelocityComponent>();

        component.max_velocity = max_velocity;
    }

    sf::Vector2f max_velocity;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(VelocityComponent);

} // namespace fck

#endif // VELOCITYCOMPONENT_H
