#ifndef SHADOW_MNLVCFKJEGDV_H
#define SHADOW_MNLVCFKJEGDV_H

#include "../knowledge_base.h"

#include "../fck/resource_cache.h"

#include <SFML/Graphics.hpp>

namespace fck
{

namespace component
{

struct Shadow
{
    std::unique_ptr<sf::Shape> shadow_shape;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Shadow> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SHADOW;
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

        if (table->contains("type"))
            type = table->at("type").as_string()->get();

        if (table->contains("rect_size"))
            rect_size = vector2::tomlArrayToVector2f(table->at("rect_size").as_array());

        if (table->contains("radius"))
            radius = table->at("radius").as_floating_point()->get();
    }

    void create(Entity &entity)
    {
        component::Shadow &component = entity.add<component::Shadow>();

        if (type == "rect")
        {
            sf::RectangleShape *shape = new sf::RectangleShape();
            shape->setFillColor(sf::Color(0, 0, 0, 110));

            shape->setSize(rect_size);

            component.shadow_shape.reset(shape);
        }
        else if (type == "circle")
        {
            sf::CircleShape *shape = new sf::CircleShape();
            shape->setFillColor(sf::Color(0, 0, 0, 110));

            shape->setPointCount(12);
            shape->setRadius(radius);

            component.shadow_shape.reset(shape);
        }

        component.shadow_shape->setPosition(position);
        component.shadow_shape->setRotation(sf::degrees(rotation));
        component.shadow_shape->setScale(scale);
        component.shadow_shape->setOrigin(origin);
    }

    sf::Vector2f position;
    float rotation = 0.0f;
    sf::Vector2f scale = {1.0f, 1.0f};
    sf::Vector2f origin;

    std::string type = "rect";
    sf::Vector2f rect_size = {0.0f, 0.0f};
    float radius = 0.0f;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Shadow);

}

} // namespace fck

#endif // SHADOW_MNLVCFKJEGDV_H
