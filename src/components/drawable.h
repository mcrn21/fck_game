#ifndef DRAWABLE_DEZGEGXGIYIU_H
#define DRAWABLE_DEZGEGXGIYIU_H

#include "drawable_animation.h"
#include "drawable_state.h"

#include "../knowledge_base.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/drawable_proxy.h"

#include <memory>

namespace fck
{

namespace component
{

struct Drawable
{
    std::unique_ptr<DrawableProxyBase> proxy;
    sf::FloatRect global_bounds;

    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;

    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Drawable> : ComponentItemBase
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
        component::Drawable &component = entity.addComponent<component::Drawable>();

        auto [drawable_proxy, drawable_state, drawable_animation]
            = KnowledgeBase::createDrawable(name);

        if (drawable_proxy)
        {
            drawable_proxy->toTransformable()->setPosition(position);
            drawable_proxy->toTransformable()->setRotation(sf::degrees(rotation));
            drawable_proxy->toTransformable()->setScale(scale);
            drawable_proxy->toTransformable()->setOrigin(origin);
            component.global_bounds = drawable_proxy->getLocalBounds();
            component.proxy.reset(drawable_proxy);
        }

        if (drawable_state)
        {
            component::DrawableState &drawable_state_component
                = entity.addComponent<component::DrawableState>();
            drawable_state_component.state.reset(drawable_state);
        }

        if (drawable_animation)
        {
            component::DrawableAnimation &drawable_animation_component
                = entity.addComponent<component::DrawableAnimation>();
            drawable_animation_component.animation.reset(drawable_animation);
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

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Drawable);

}

} // namespace fck

#endif // DRAWABLE_DEZGEGXGIYIU_H
