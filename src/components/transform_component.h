#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include <SFML/Graphics/Transformable.hpp>

#include <cstdint>
#include <functional>
#include <vector>

namespace fck
{

struct TransformComponent
{
    sf::Transformable transform;

    Entity parent;
    std::vector<Entity> children;
};

template<>
struct KnowledgeBase::ComponentItem<TransformComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::TRANSFORM;
    }

    void init(toml::table *table)
    {
        if (table->contains("position"))
            position = vector2::tomlArrayToVector2f(table->at("position").as_array());

        if (table->contains("rotation"))
            rotation = table->at("rotation").as_floating_point()->get();

        if (table->contains("scale"))
            scale = vector2::tomlArrayToVector2f(table->at("scale").as_array());

        if (table->contains("origin"))
            origin = vector2::tomlArrayToVector2f(table->at("origin").as_array());
    }

    void create(Entity &entity)
    {
        TransformComponent &component = entity.addComponent<TransformComponent>();

        component.transform.setPosition(position);
        component.transform.setRotation(sf::degrees(rotation));
        component.transform.setScale(scale);
        component.transform.setOrigin(origin);
    }

    sf::Vector2f position;
    float rotation = 0.0f;
    sf::Vector2f scale;
    sf::Vector2f origin;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(TransformComponent);

} // namespace fck

#endif // TRANSFORMCOMPONENT_H
