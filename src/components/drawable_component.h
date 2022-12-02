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
    sf::FloatRect global_content_bounds;
    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;
    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;
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
        type = drawable_type::fromString(table->at("type").as_string()->get());
        name = table->at("name").as_string()->get();

        if (table->contains("z_order"))
            z_order = table->at("z_order").as_integer()->get();

        if (table->contains("z_order_fill_y_coordinate"))
            z_order_fill_y_coordinate = table->at("z_order_fill_y_coordinate").as_boolean()->get();
    }

    void create(Entity &entity)
    {
        DrawableComponent &component = entity.addComponent<DrawableComponent>();

        if (type == drawable_type::SPRITE)
        {
            auto sprite = KnowledgeBase::createDrawable(name);
            component.global_bounds = sprite.first->localBounds();
            component.drawable.reset(sprite.first);

            if (sprite.second)
            {
                DrawableAnimationComponent &drawable_animation_component
                    = entity.addComponent<DrawableAnimationComponent>();
                drawable_animation_component.drawable_animation.reset(sprite.second);
            }
            component.z_order = z_order;
            component.z_order_fill_y_coordinate = z_order_fill_y_coordinate;

            return;
        }
    }

    drawable_type::Type type = drawable_type::NO_TYPE;
    std::string name;
    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(DrawableComponent);

} // namespace fck

#endif // DRAWABLECOMPONENT_H