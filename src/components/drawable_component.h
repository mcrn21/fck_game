#ifndef DRAWABLECOMPONENT_H
#define DRAWABLECOMPONENT_H

#include "drawable_animation_component.h"

#include "../knowledge_base.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/drawable.h"

#include <memory>

namespace fck
{

struct DrawableComponent
{
    std::unique_ptr<Drawable> drawable;
    sf::FloatRect global_bounds;

    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;

    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;
};

template<>
struct KnowledgeBase::ComponentItem<DrawableComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::DRAWABLE;
    }

    void init(toml::table *table)
    {
        name = table->at("name").as_string()->get();

        if (table->contains("position"))
            position = vector2::tomlArrayToVector2f(table->at("position").as_array());

        if (table->contains("rotation"))
            rotation = table->at("rotation").as_floating_point()->get();

        if (table->contains("scale"))
            scale = vector2::tomlArrayToVector2f(table->at("scale").as_array());

        if (table->contains("origin"))
            origin = vector2::tomlArrayToVector2f(table->at("origin").as_array());

        if (table->contains("z_order"))
            z_order = table->at("z_order").as_integer()->get();

        if (table->contains("z_order_fill_y_coordinate"))
            z_order_fill_y_coordinate = table->at("z_order_fill_y_coordinate").as_boolean()->get();
    }

    void create(Entity &entity)
    {
        DrawableComponent &component = entity.addComponent<DrawableComponent>();

        auto drawable = KnowledgeBase::createDrawable(name);
        component.global_bounds = drawable.first->localBounds();
        component.drawable.reset(drawable.first);

        if (drawable.first)
        {
            component.drawable->setPosition(position);
            component.drawable->setRotation(sf::degrees(rotation));
            component.drawable->setScale(scale);
            component.drawable->setOrigin(origin);
        }

        if (drawable.second)
        {
            DrawableAnimationComponent &drawable_animation_component
                = entity.addComponent<DrawableAnimationComponent>();
            drawable_animation_component.drawable_animation.reset(drawable.second);
        }

        component.z_order = z_order;
        component.z_order_fill_y_coordinate = z_order_fill_y_coordinate;
    }

    std::string name;

    sf::Vector2f position;
    float rotation = 0.0f;
    sf::Vector2f scale = {1.0f, 1.0f};
    sf::Vector2f origin;

    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(DrawableComponent);

} // namespace fck

#endif // DRAWABLECOMPONENT_H
