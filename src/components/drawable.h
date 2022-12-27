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
        for (const auto &it : *table)
        {
            if (it.second.is_table())
            {
                drawable_item.reset(KnowledgeBase::instance().loadDrawableFromTable(
                    it.first.data(), it.second.as_table()));
            }
            else
            {
                std::string field_name = it.first.data();

                if (field_name == "position")
                    position = vector2::tomlArrayToVector2f(it.second.as_array());

                if (field_name == "rotation")
                    rotation = it.second.as_floating_point()->get();

                if (field_name == "scale")
                    scale = vector2::tomlArrayToVector2f(it.second.as_array());

                if (field_name == "origin")
                    origin = vector2::tomlArrayToVector2f(it.second.as_array());

                if (field_name == "z_order")
                    z_order = it.second.as_integer()->get();

                if (field_name == "z_order_fill_y_coordinate")
                    z_order_fill_y_coordinate = it.second.as_boolean()->get();
            }
        }
    }

    void create(Entity &entity)
    {
        component::Drawable &component = entity.add<component::Drawable>();

        if (drawable_item)
        {
            auto [drawable_proxy, drawable_state, drawable_animation] = drawable_item->create();

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
                    = entity.add<component::DrawableState>();
                drawable_state_component.state.reset(drawable_state);
            }

            if (drawable_animation)
            {
                component::DrawableAnimation &drawable_animation_component
                    = entity.add<component::DrawableAnimation>();
                drawable_animation_component.animation.reset(drawable_animation);
            }
        }

        component.z_order = z_order;
        component.z_order_fill_y_coordinate = z_order_fill_y_coordinate;
    }

    std::string name;

    std::unique_ptr<DrawableItemBase> drawable_item;

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
